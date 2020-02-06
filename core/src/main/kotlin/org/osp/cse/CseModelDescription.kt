package org.osp.cse

import java.lang.IllegalArgumentException

class CseModelDescription(
        val modelName: String,
        val uuid: String,
        val description: String = "",
        val author: String = "",
        val version: String = "",
        val variables: List<CseVariableDescription>
) {

    fun getVariable(variableName: String): CseVariableDescription {
        return variables.find { it.name == variableName }
                ?: throw IllegalArgumentException("No variable with name='$variableName' found in model '$modelName'!")
    }

    fun getVariable(vr: Long, type: CseVariableType): CseVariableDescription {
        return variables.find { it.valueReference == vr && it.type == type }
                ?: throw IllegalArgumentException("No variable with vr=$vr of type=$type found in model '$modelName'!")
    }

    override fun toString(): String {
        return "CseModelDescription(modelName='$modelName', uuid='$uuid', description='$description', author='$author', version='$version', numVariables=${variables.size})"
    }

}
