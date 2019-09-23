package org.osp.cse

import java.util.*
import java.util.concurrent.atomic.AtomicLong

abstract class JvmSlave(
        val name: String
) {

    val uuid: String = UUID.randomUUID().toString()

    private val variables_ = mutableListOf<CseVariableDescription>()

    val variables: List<CseVariableDescription> = variables_

    protected fun registerVariables(vararg variable: Var<*>) {
        variables_.addAll(variable.map { it.build() })
    }

//    protected fun registerReal(name: String, getter: () -> Double, setter: () -> )

    abstract fun enterInitialisationMode(startTime: Double)

    abstract fun exitInitialisationMode()

    abstract fun doStep(currentTime: Double, stepSize: Double): Boolean

    abstract fun terminate()

//    fun readReal(vr)

}

sealed class Var<E>(
        val name: String,
        val type: CseVariableType,
        val getter: () -> E,
        val setter: ((E) -> Unit)? = null
) {

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

    fun build(): CseVariableDescription {
        return CseVariableDescription(
                name,
                vr.getAndIncrement(),
                type,
                causality ?: CseVariableCausality.LOCAL,
                variability ?: CseVariableVariability.CONTINUOUS
        )
    }

    private companion object {

        val vr = AtomicLong(0)

    }

}


class RealVar @JvmOverloads constructor(
        name: String,
        getter: () -> Double,
        setter: ((Double) -> Unit)? = null
) : Var<Double>(name, CseVariableType.REAL, getter, setter)
