package org.osp.cosim

import org.osp.cosim.jni.CosimLibrary
import org.osp.cosim.jni.ExecutionPtr
import org.osp.cosim.jni.SlavePtr
import java.io.Closeable

open class CosimSlave internal constructor(
        val slaveRef: SlaveRef,
        val instanceName: String,
        private val executionPtr: ExecutionPtr
) : Closeable {

    val modelDescription: CosimModelDescription by lazy {
        CosimLibrary.getModelDescription(executionPtr, slaveRef)
    }

    fun getVariable(name: String): CosimVariableDescription {
        return modelDescription.getVariable(name)
    }

    fun getVariable(vr: Long, type: CosimVariableType): CosimVariableDescription {
        return modelDescription.getVariable(vr, type)
    }

    fun setInitialRealValue(vr: Long, value: Double) {
        CosimLibrary.setInitialRealValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialRealValue(name: String, value: Double) {
        setInitialRealValue(getVariable(name).valueReference, value)
    }

    fun setInitialIntegerValue(vr: Long, value: Int) {
        CosimLibrary.setInitialIntegerValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialIntegerValue(name: String, value: Int) {
        setInitialIntegerValue(getVariable(name).valueReference, value)
    }

    fun setInitialBooleanValue(vr: Long, value: Boolean) {
        CosimLibrary.setInitialBooleanValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialBooleanValue(name: String, value: Boolean) {
        setInitialBooleanValue(getVariable(name).valueReference, value)
    }

    fun setInitialStringValue(vr: Long, value: String) {
        CosimLibrary.setInitialStringValue(executionPtr, slaveRef, vr, value);
    }

    fun setInitialRealValue(name: String, value: String) {
        setInitialStringValue(getVariable(name).valueReference, value)
    }

    override fun close() {
        // nothing to do
    }

    override fun toString(): String {
        return "CosimSlave(name='$instanceName', index=$slaveRef)"
    }

}

class CosimLocalSlave(
        slaveRef: SlaveRef,
        instanceName: String,
        executionPtr: ExecutionPtr,
        private var slavePtr: SlavePtr
) : CosimSlave(slaveRef, instanceName, executionPtr), Closeable {

    override fun close() {
        if (slavePtr != 0L) {
            CosimLibrary.destroySlave(slavePtr)
            slavePtr = 0L
        }
    }

}

class CosimSlaveInfo(
        var slaveRef: SlaveRef,
        val instanceName: String
) {

    override fun toString(): String {
        return "CosimSlaveInfo(name=$instanceName, index=$slaveRef)"
    }

}
