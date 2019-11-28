package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ExecutionPtr
import org.osp.cse.jni.SlavePtr
import java.io.Closeable
import java.lang.IllegalArgumentException

open class CseSlave internal constructor(
        val index: Int,
        val name: String,
        private val executionPtr: ExecutionPtr
) : Closeable {

    val modelDescription: CseModelDescription by lazy {
        CseLibrary.getModelDescription(executionPtr, index)
    }

    fun getVariable(name: String): CseVariableDescription {
        return modelDescription.getVariable(name)
                ?: throw IllegalArgumentException("No variable with name=$name found!")
    }

    fun getVariable(vr: Long, type: CseVariableType): CseVariableDescription {
        return modelDescription.getVariable(vr, type)
                ?: throw IllegalArgumentException("No variable with valueReference=$vr and type=$type found!")
    }

    fun setInitialRealValue(vr: Long, value: Double) {
        CseLibrary.setInitialRealValue(executionPtr, index, vr, value);
    }

    fun setInitialRealValue(name: String, value: Double) {
        getVariable(name).valueReference.let {
            setInitialRealValue(it, value)
        }
    }

    fun setInitialIntegerValue(vr: Long, value: Int) {
        CseLibrary.setInitialIntegerValue(executionPtr, index, vr, value);
    }

    fun setInitialIntegerValue(name: String, value: Int) {
        getVariable(name).valueReference.let {
            setInitialIntegerValue(it, value)
        }
    }

    fun setInitialBooleanValue(vr: Long, value: Boolean) {
        CseLibrary.setInitialBooleanValue(executionPtr, index, vr, value);
    }

    fun setInitialBooleanValue(name: String, value: Boolean) {
        getVariable(name).valueReference.let {
            setInitialBooleanValue(it, value)
        }
    }

    fun setInitialStringValue(vr: Long, value: String) {
        CseLibrary.setInitialStringValue(executionPtr, index, vr, value);
    }

    fun setInitialRealValue(name: String, value: String) {
        getVariable(name).valueReference.let {
            setInitialStringValue(it, value)
        }
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
