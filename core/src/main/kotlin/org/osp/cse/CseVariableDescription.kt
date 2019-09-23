package org.osp.cse

class CseVariableDescription internal constructor(
        val name: String,
        val valueReference: Long,
        val type: CseVariableType,
        val causality: CseVariableCausality,
        val variability: CseVariableVariability
) {

    constructor(
            name: String,
            valueReference: Long,
            variableType: Int,
            variableCausality: Int,
            variableVariability: Int
    ) : this(
            name,
            valueReference,
            CseVariableType.valueOf(variableType),
            CseVariableCausality.valueOf(variableCausality),
            CseVariableVariability.valueOf(variableVariability))

    override fun toString(): String {
        return "CseVariableDescription(name='$name', valueReference=$valueReference, variableType=$type, causality=$causality, variability=$variability)"
    }

}

enum class CseVariableType(
        internal val code: Int
) {

    REAL(0),
    INTEGER(1),
    STRING(2),
    BOOLEAN(3);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseVariableType {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CseVariableCausality(
        private val code: Int
) {

    INPUT(0),
    PARAMETER(1),
    OUTPUT(2),
    CALCULATED_PARAMETER(3),
    LOCAL(4),
    INDEPENDENT(5);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseVariableCausality {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CseVariableVariability(
        private val code: Int
) {

    CONSTANT(0),
    FIXED(1),
    TUNABLE(2),
    DISCRETE(3),
    CONTINUOUS(4);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseVariableVariability {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }
}
