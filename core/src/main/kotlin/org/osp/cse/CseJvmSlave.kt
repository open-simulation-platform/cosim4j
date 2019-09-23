package org.osp.cse

import java.util.*
import java.util.concurrent.atomic.AtomicLong

abstract class CseJvmSlave {

    private val model: Model = define()

    val modelDescription by lazy {
        model.toCseModelDescription()
    }

    protected abstract fun define(): Model

    abstract fun setup(startTime: Double)

    open fun onBeforeStart() {}

    abstract fun doStep(currentTime: Double, stepSize: Double)

    open fun terminate() {}

    fun getReal(vr: LongArray): DoubleArray {
        return DoubleArray(vr.size) { i ->
            (model.variables.getValue(vr[i]) as RealVar).let {
                it.getter()
            }
        }
    }

    fun setReal(vr: LongArray, values: DoubleArray) {
        for (i in vr.indices) {
            (model.variables.getValue(vr[i]) as RealVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    fun getInteger(vr: LongArray): IntArray {
        return IntArray(vr.size) { i ->
            (model.variables.getValue(vr[i]) as IntVar).let {
                it.getter()
            }
        }
    }

    fun setInteger(vr: LongArray, values: IntArray) {
        for (i in vr.indices) {
            (model.variables.getValue(vr[i]) as IntVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    fun getBoolean(vr: LongArray): BooleanArray {
        return BooleanArray(vr.size) { i ->
            (model.variables.getValue(vr[i]) as BoolVar).let {
                it.getter()
            }
        }
    }

    fun setBoolean(vr: LongArray, values: BooleanArray) {
        for (i in vr.indices) {
            (model.variables.getValue(vr[i]) as BoolVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    fun getString(vr: LongArray): Array<String> {
        return Array(vr.size) { i ->
            (model.variables.getValue(vr[i]) as StringVar).let {
                it.getter()
            }
        }
    }

    fun setString(vr: LongArray, values: Array<String>) {
        for (i in vr.indices) {
            (model.variables.getValue(vr[i]) as StringVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    protected fun model(name: String): Model = Model(name)

    @JvmOverloads
    protected fun real(name: String, getter: () -> Double, setter: ((Double) -> Unit)? = null): Var = RealVar(name, getter, setter)

    @JvmOverloads
    protected fun integer(name: String, getter: () -> Int, setter: ((Int) -> Unit)? = null): Var = IntVar(name, getter, setter)

    @JvmOverloads
    protected fun boolean(name: String, getter: () -> Boolean, setter: ((Boolean) -> Unit)? = null): Var = BoolVar(name, getter, setter)

    @JvmOverloads
    protected fun string(name: String, getter: () -> String, setter: ((String) -> Unit)? = null): Var = StringVar(name, getter, setter)

}

class Model(
        private val name: String
) {

    private var author: String = ""
    private var description: String = ""
    private var version: String = ""
    private val uuid = UUID.randomUUID().toString()

    internal val variables = mutableMapOf<Long, Var>()

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

    fun add(variable: Var): Model {
        variables[variable.vr] = variable
        return this
    }

    fun toCseModelDescription(): CseModelDescription {
        return CseModelDescription(name, uuid, description, author, version, variables.values.map { it.toCseVariableDescription() })
    }

}

sealed class Var(
        val name: String,
        val type: CseVariableType
) {

    internal val vr = vr_gen.getAndIncrement()

    private var causality: CseVariableCausality? = null
    private var variability: CseVariableVariability? = null

    fun causality(causality: CseVariableCausality): Var {
        this.causality = causality
        return this
    }

    fun variability(variableVariability: CseVariableVariability): Var {
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
        val getter: () -> Double,
        val setter: ((Double) -> Unit)? = null
) : Var(name, CseVariableType.REAL)

class IntVar @JvmOverloads constructor(
        name: String,
        val getter: () -> Int,
        val setter: ((Int) -> Unit)? = null
) : Var(name, CseVariableType.INTEGER)

class BoolVar @JvmOverloads constructor(
        name: String,
        val getter: () -> Boolean,
        val setter: ((Boolean) -> Unit)? = null
) : Var(name, CseVariableType.BOOLEAN)

class StringVar @JvmOverloads constructor(
        name: String,
        val getter: () -> String,
        val setter: ((String) -> Unit)? = null
) : Var(name, CseVariableType.STRING)

