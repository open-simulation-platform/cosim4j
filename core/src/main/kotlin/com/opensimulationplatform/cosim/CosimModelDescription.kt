package com.opensimulationplatform.cosim

class CosimModelDescription(
    val modelName: String,
    val uuid: String,
    val description: String = "",
    val author: String = "",
    val version: String = "",
    val variables: List<CosimVariableDescription>
) {

    fun getVariable(variableName: String): CosimVariableDescription {
        return variables.find { it.name == variableName }
            ?: throw IllegalArgumentException("No variable with name='$variableName' found in model '$modelName'!")
    }

    fun getVariable(vr: Long, type: CosimVariableType): CosimVariableDescription {
        return variables.find { it.valueReference == vr && it.type == type }
            ?: throw IllegalArgumentException("No variable with vr=$vr of type=$type found in model '$modelName'!")
    }

    override fun toString(): String {
        return "CosimModelDescription(modelName='$modelName', uuid='$uuid', description='$description', author='$author', version='$version', numVariables=${variables.size})"
    }

}
