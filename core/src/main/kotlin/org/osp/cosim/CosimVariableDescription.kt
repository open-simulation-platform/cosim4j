package org.osp.cosim

class CosimVariableDescription internal constructor(
        val name: String,
        val valueReference: Long,
        val type: CosimVariableType,
        causality: CosimVariableCausality? = null,
        variability: CosimVariableVariability? = null
) {

    val causality = causality ?: CosimVariableCausality.LOCAL
    val variability = variability ?: CosimVariableVariability.CONTINUOUS

    fun getTypeId() = this.type.code
    fun getCausalityId() = this.causality.code
    fun getVariabilityId() = this.variability.code

    constructor(
            name: String,
            valueReference: Long,
            variableType: Int,
            variableCausality: Int,
            variableVariability: Int
    ) : this(
            name,
            valueReference,
            CosimVariableType.valueOf(variableType),
            CosimVariableCausality.valueOf(variableCausality),
            CosimVariableVariability.valueOf(variableVariability))

    override fun toString(): String {
        return "CosimVariableDescription(name='$name', valueReference=$valueReference, variableType=$type, causality=$causality, variability=$variability)"
    }

}

enum class CosimVariableType(
        internal val code: Int
) {

    REAL(0),
    INTEGER(1),
    BOOLEAN(2),
    STRING(3),
    ENUMERATION(4);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CosimVariableType {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CosimVariableCausality(
        internal val code: Int
) {

    PARAMETER(0),
    CALCULATED_PARAMETER(1),
    INPUT(2),
    OUTPUT(3),
    LOCAL(4);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CosimVariableCausality {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CosimVariableVariability(
        internal val code: Int
) {

    CONSTANT(0),
    FIXED(1),
    TUNABLE(2),
    DISCRETE(3),
    CONTINUOUS(4);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CosimVariableVariability {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }
}
