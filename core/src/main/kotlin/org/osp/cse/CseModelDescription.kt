package org.osp.cse

class CseModelDescription(
        val name: String,
        val uuid: String,
        val description: String,
        val author: String,
        val version: String,
        val variables: List<CseVariableDescription>

) {

    fun getVariable(name: String): CseVariableDescription? {
        return variables.find { it.name == name }
    }

    fun getVariable(vr: Long, type: CseVariableType): CseVariableDescription? {
        return variables.find { it.valueReference == vr && it.type == type }
    }

    override fun toString(): String {
        return "CseModelDescription(name='$name', uuid='$uuid', description='$description', author='$author', version='$version', numVariables=${variables.size})"
    }
}
