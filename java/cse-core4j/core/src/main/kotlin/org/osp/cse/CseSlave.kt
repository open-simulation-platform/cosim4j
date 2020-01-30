package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ExecutionPtr
import org.osp.cse.jni.SlavePtr
import java.io.Closeable

open class CseSlave internal constructor(
        val slaveRef: SlaveRef,
        val instanceName: String,
        private val executionPtr: ExecutionPtr
) : Closeable {

    val modelDescription: CseModelDescription by lazy {
        CseLibrary.getModelDescription(executionPtr, slaveRef)
    }

    fun getVariable(name: String): CseVariableDescription {
        return modelDescription.getVariable(name)
    }

    fun getVariable(vr: Long, type: CseVariableType): CseVariableDescription {
        return modelDescription.getVariable(vr, type)
    }

    fun setInitialRealValue(vr: Long, value: Double) {
        CseLibrary.setInitialRealValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialRealValue(name: String, value: Double) {
        setInitialRealValue(getVariable(name).valueReference, value)
    }

    fun setInitialIntegerValue(vr: Long, value: Int) {
        CseLibrary.setInitialIntegerValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialIntegerValue(name: String, value: Int) {
        setInitialIntegerValue(getVariable(name).valueReference, value)
    }

    fun setInitialBooleanValue(vr: Long, value: Boolean) {
        CseLibrary.setInitialBooleanValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialBooleanValue(name: String, value: Boolean) {
        setInitialBooleanValue(getVariable(name).valueReference, value)
    }

    fun setInitialStringValue(vr: Long, value: String) {
        CseLibrary.setInitialStringValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialRealValue(name: String, value: String) {
        setInitialStringValue(getVariable(name).valueReference, value)
    }

    override fun close() {
        // nothing to do
    }

    override fun toString(): String {
        return "CseSlave(name='$instanceName', index=$slaveRef)"
    }

}

class CseLocalSlave(
        slaveRef: SlaveRef,
        instanceName: String,
        executionPtr: ExecutionPtr,
        private var slavePtr: SlavePtr
) : CseSlave(slaveRef, instanceName, executionPtr), Closeable {

    override fun close() {
        if (slavePtr != 0L) {
            CseLibrary.destroySlave(slavePtr)
            slavePtr = 0L
        }
    }

}

class CseSlaveInfo(
        var slaveRef: SlaveRef,
        val instanceName: String
) {

    override fun toString(): String {
        return "CseSlaveInfo(name=$instanceName, index=$slaveRef)"
    }

}
