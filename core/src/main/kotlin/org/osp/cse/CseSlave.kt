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

    fun getReal(observer: CseLastValueObserver, vr: Long): Double? {
        return observer.getReal(this, vr)
    }

    fun getReal(observer: CseLastValueObserver, vr: LongArray, ref: DoubleArray): Boolean {
        return observer.getReal(this, vr, ref)
    }

    fun getInteger(observer: CseLastValueObserver, vr: Long): Int? {
        return observer.getInteger(this, vr)
    }

    fun getInteger(observer: CseLastValueObserver, vr: LongArray, ref: IntArray): Boolean {
        return observer.getInteger(this, vr, ref)
    }

    fun getBoolean(observer: CseLastValueObserver, vr: Long): Boolean? {
        return observer.getBoolean(this, vr)
    }

    fun getBoolean(observer: CseLastValueObserver, vr: LongArray, ref: BooleanArray): Boolean {
        return observer.getBoolean(this, vr, ref)
    }

    fun getString(observer: CseLastValueObserver, vr: Long): String? {
        return observer.getString(this, vr)
    }

    fun getString(observer: CseLastValueObserver, vr: LongArray, ref: Array<String>): Boolean {
        return observer.getString(this, vr, ref)
    }

    fun setReal(manipulator: CseOverrideManipulator, vr: Long, value: Double): Boolean {
        return manipulator.setReal(index, vr, value)
    }

    fun setReal(manipulator: CseOverrideManipulator, vr: LongArray, values: DoubleArray): Boolean {
        return manipulator.setReal(index, vr, values)
    }

    fun setInteger(manipulator: CseOverrideManipulator, vr: Long, value: Int): Boolean {
        return manipulator.setInteger(index, vr, value)
    }

    fun setInteger(manipulator: CseOverrideManipulator, vr: LongArray, values: IntArray): Boolean {
        return manipulator.setInteger(index, vr, values)
    }

    fun setBoolean(manipulator: CseOverrideManipulator, vr: Long, value: Boolean): Boolean {
        return manipulator.setBoolean(index, vr, value)
    }

    fun setBoolean(manipulator: CseOverrideManipulator, vr: LongArray, values: BooleanArray): Boolean {
        return manipulator.setBoolean(index, vr, values)
    }

    fun setString(manipulator: CseOverrideManipulator, vr: Long, value: String): Boolean {
        return manipulator.setString(this, vr, value)
    }

    fun setString(manipulator: CseOverrideManipulator, vr: LongArray, values: Array<String>): Boolean {
        return manipulator.setString(index, vr, values)
    }

    fun getRealSamples(observer: CseLastValueObserver, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return observer.getRealSamples(this, vr, stepNumber, nSamples)
    }

    fun getIntegerSamples(observer: CseLastValueObserver, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return observer.getIntegerSamples(this, vr, stepNumber, nSamples)
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
        val name: String,
        val source: String
) {

    override fun toString(): String {
        return "CseSlaveInfo(name=$name, index=$index, source=$source)"
    }

}
