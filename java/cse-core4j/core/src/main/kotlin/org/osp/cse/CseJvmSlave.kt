package org.osp.cse

import java.util.*
import java.util.concurrent.atomic.AtomicLong

@Target(AnnotationTarget.CLASS)
@Retention(AnnotationRetention.RUNTIME)
annotation class SlaveInfo(
        val name: String,
        val author: String = "",
        val version: String = "",
        val description: String = ""
)

@Target(AnnotationTarget.FIELD)
@Retention(AnnotationRetention.RUNTIME)
annotation class ScalarVariable(
        val causality: CseVariableCausality = CseVariableCausality.LOCAL,
        val variability: CseVariableVariability = CseVariableVariability.CONTINUOUS
)

abstract class CseJvmSlave {

    private lateinit var modelInfo: SlaveInfo
    private val variables = mutableMapOf<Long, Var<*>>()

    private val modelDescription by lazy {
        define()
        val uuid = UUID.randomUUID().toString()
        CseModelDescription("${modelInfo.name}_$uuid", uuid, modelInfo.description, modelInfo.author, modelInfo.version, variables.values.map { it.toCseVariableDescription() })
    }

    private fun define() {
        var vrCounter = 0L

        modelInfo = javaClass.getAnnotation(SlaveInfo::class.java)
                ?: throw IllegalStateException("No ${SlaveInfo::class} annotation present!")

        javaClass.declaredFields.forEach { field ->

            field.getAnnotation(ScalarVariable::class.java)?.also { v ->

                field.isAccessible = true

                when (val type = field.type) {
                    Int::class, Int::class.java -> {
                        variables[vrCounter] = Var(vrCounter++, field.name, CseVariableType.INTEGER, v.causality, v.variability, { field.getInt(this) }, { field.setInt(this, it) })
                    }
                    Double::class, Double::class.java -> {
                        variables[vrCounter] = Var(vrCounter++, field.name, CseVariableType.REAL, v.causality, v.variability, { field.getDouble(this) }, { field.setDouble(this, it) })
                    }
                    Boolean::class, Boolean::class.java -> {
                        variables[vrCounter] = Var(vrCounter++, field.name, CseVariableType.BOOLEAN, v.causality, v.variability, { field.getBoolean(this) }, { field.setBoolean(this, it) })
                    }
                    String::class, String::class.java -> {
                        variables[vrCounter] = Var(vrCounter++, field.name, CseVariableType.STRING, v.causality, v.variability, { field.get(this) }, { field.set(this, it) })
                    }
                    IntArray::class.java -> {
                        (field.get(this) as IntArray).also { array ->
                            for (i in array.indices) {
                                variables[vrCounter] = Var(vrCounter++, "${field.name}_$i", CseVariableType.INTEGER, v.causality, v.variability, { array[i] }, { array[i] = it })
                            }
                        }
                    }
                    DoubleArray::class.java -> {
                        (field.get(this) as DoubleArray).also { array ->
                            for (i in array.indices) {
                                variables[vrCounter] = Var(vrCounter++, "${field.name}_$i", CseVariableType.REAL, v.causality, v.variability, { array[i] }, { array[i] = it })
                            }
                        }
                    }
                    BooleanArray::class.java -> {
                        (field.get(this) as BooleanArray).also { array ->
                            for (i in array.indices) {
                                variables[vrCounter] = Var(vrCounter++, "${field.name}_$i", CseVariableType.BOOLEAN, v.causality, v.variability, { array[i] }, { array[i] = it })
                            }
                        }
                    }
                    Array<String>::class.java -> {
                        (field.get(this) as Array<String>).also { array ->
                            for (i in array.indices) {
                                variables[vrCounter] = Var(vrCounter++, "${field.name}_$i", CseVariableType.STRING, v.causality, v.variability, { array[i] }, { array[i] = it })
                            }
                        }
                    }
                    else -> throw IllegalStateException("Unsupported ScalarVariable type: $type")
                }

            }

        }
    }

    abstract fun setup(startTime: Double)

    open fun onBeforeStart() {}

    abstract fun doStep(currentTime: Double, stepSize: Double)

    open fun terminate() {}

    fun getReal(vr: LongArray): DoubleArray {
        return DoubleArray(vr.size) { i ->
            (variables.getValue(vr[i]) as RealVar).let {
                it.getter()
            }
        }
    }

    fun setReal(vr: LongArray, values: DoubleArray) {
        for (i in vr.indices) {
            (variables.getValue(vr[i]) as RealVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    fun getInteger(vr: LongArray): IntArray {
        return IntArray(vr.size) { i ->
            (variables.getValue(vr[i]) as IntVar).let {
                it.getter()
            }
        }
    }

    fun setInteger(vr: LongArray, values: IntArray) {
        for (i in vr.indices) {
            (variables.getValue(vr[i]) as IntVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    fun getBoolean(vr: LongArray): BooleanArray {
        return BooleanArray(vr.size) { i ->
            (variables.getValue(vr[i]) as BoolVar).let {
                it.getter()
            }
        }
    }

    fun setBoolean(vr: LongArray, values: BooleanArray) {
        for (i in vr.indices) {
            (variables.getValue(vr[i]) as BoolVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    fun getString(vr: LongArray): Array<String> {
        return Array(vr.size) { i ->
            (variables.getValue(vr[i]) as StringVar).let {
                it.getter()
            }
        }
    }

    fun setString(vr: LongArray, values: Array<String>) {
        for (i in vr.indices) {
            (variables.getValue(vr[i]) as StringVar).apply {
                setter?.invoke(values[i])
            }
        }
    }

    private companion object {
        private val counter = AtomicLong(0)
    }

}


class Var<T>(
        val vr: Long,
        val name: String,
        val type: CseVariableType,
        val causality: CseVariableCausality,
        val variability: CseVariableVariability,
        val getter: () -> T,
        val setter: ((T) -> Unit)?
) {

    fun toCseVariableDescription(): CseVariableDescription {
        return CseVariableDescription(name, vr, type, causality, variability)
    }

}

typealias IntVar = Var<Int>
typealias RealVar = Var<Double>
typealias BoolVar = Var<Boolean>
typealias StringVar = Var<String>
