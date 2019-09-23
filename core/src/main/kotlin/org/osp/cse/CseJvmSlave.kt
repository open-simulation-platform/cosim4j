package org.osp.cse

import java.util.*
import java.util.concurrent.atomic.AtomicLong

abstract class CseJvmSlave {

    private val model = define()

    val modelDescription by lazy {
        model.toCseModelDescription()
    }

    protected abstract fun define(): Model

    abstract fun setup(startTime: Double)

    open fun onBeforeStart() {}

    abstract fun doStep(currentTime: Double, stepSize: Double)

    open fun terminate() {}

    private fun getReal(vr: LongArray, ref: DoubleArray) {

    }

    protected fun model(name: String): Model = Model(name)

    protected fun real(name: String, getter: () -> Double): Var<*> = RealVar(name, getter)

}

class Model(
        val name: String
) {

    private var author: String = ""
    private var description: String = ""
    private var version: String = ""
    private val uuid = UUID.randomUUID().toString()

    private val variables = mutableMapOf<Long, Var<*>>()

    fun author(value: String): Model {
        author = value
        return this
    }

    fun version(value: String): Model {
        version = value
        return this
    }

    fun description(value: String): Model {
        description = value
        return this
    }

    fun add(variable: Var<*>): Model {
        variables[variable.vr] = variable
        return this
    }

    fun toCseModelDescription(): CseModelDescription {
        return CseModelDescription(name, uuid, description, author, version, variables.values.map { it.toCseVariableDescription() })
    }

}

sealed class Var<E>(
        val name: String,
        val type: CseVariableType,
        val getter: () -> E,
        val setter: ((E) -> Unit)? = null
) {

    internal val vr = vr_gen.getAndIncrement()

    private var causality: CseVariableCausality? = null
    private var variability: CseVariableVariability? = null

    fun causality(causality: CseVariableCausality): Var<E> {
        this.causality = causality
        return this
    }

    fun variability(variableVariability: CseVariableVariability): Var<E> {
        this.variability = variableVariability
        return this
    }

    fun toCseVariableDescription(): CseVariableDescription {
        return CseVariableDescription(name, vr, type, causality, variability)
    }

    private companion object {

        val vr_gen = AtomicLong(0)

    }

}


class RealVar @JvmOverloads constructor(
        name: String,
        getter: () -> Double,
        setter: ((Double) -> Unit)? = null
) : Var<Double>(name, CseVariableType.REAL, getter, setter)
