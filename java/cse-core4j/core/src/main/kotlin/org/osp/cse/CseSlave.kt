package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ExecutionPtr
import org.osp.cse.jni.SlavePtr
import java.io.Closeable

open class CseSlave internal constructor(
        val index: Int,
        val name: String,
        private val executionPtr: ExecutionPtr
) : Closeable {

    val modelDescription: CseModelDescription by lazy {
        CseLibrary.getModelDescription(executionPtr, index)
    }

    fun getVariable(name: String): CseVariableDescription? {
        return modelDescription.getVariable(name)
    }

    fun getVariable(vr: Long, type: CseVariableType): CseVariableDescription? {
        return modelDescription.getVariable(vr, type)
    }

    override fun close() {
        // nothing to do
    }

    override fun toString(): String {
        return "CseSlave(name='$name', index=$index)"
    }

}

class CseLocalSlave(
        index: Int,
        name: String,
        executionPtr: ExecutionPtr,
        private var slavePtr: SlavePtr
) : CseSlave(index, name, executionPtr), Closeable {

    override fun close() {
        if (slavePtr != 0L) {
            CseLibrary.destroySlave(slavePtr)
            slavePtr = 0L
        }
    }

}

class CseSlaveInfo(
        var index: Int,
        val name: String
) {

    override fun toString(): String {
        return "CseSlaveInfo(name=$name, index=$index)"
    }

}
