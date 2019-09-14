package org.osp.cse

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
