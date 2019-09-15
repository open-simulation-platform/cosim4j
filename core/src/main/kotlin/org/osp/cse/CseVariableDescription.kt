package org.osp.cse

import java.lang.IllegalArgumentException

class CseVariableDescription internal constructor(
        val name: String,
        val valueReference: Long,
        private var variableType: Int,
        private var variableCausality: Int,
        private var variableVariability: Int
) {

    val type: CseVariableType by lazy { CseVariableType.valueOf(variableType) }
    val causality: CseVariableCausality by lazy { CseVariableCausality.valueOf(variableCausality) }
    val variability: CseVariableVariability by lazy { CseVariableVariability.valueOf(variableVariability) }

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
