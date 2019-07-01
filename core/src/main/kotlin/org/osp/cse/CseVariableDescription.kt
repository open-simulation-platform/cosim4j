package org.osp.cse

class CseVariableDescription internal constructor() {

    var valueReference: Long = -1
        private set

    var name: String = ""

    private var variableType: Int = -1
    private var variableCausality: Int = -1
    private var variableVariability: Int = -1

    val type: CseVariableType by lazy { CseVariableType.valueOf(variableType) }
    val causality: CseVariableCausality by lazy { CseVariableCausality.valueOf(variableCausality) }
    val variability: CseVariableVariability by lazy { CseVariableVariability.valueOf(variableVariability) }

    override fun toString(): String {
        return "CseVariableDescription(name='$name', valueReference=$valueReference, variableType=$type, causality=$causality, variability=$variability)"
    }

}
