package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ManipulatorPtr
import java.io.Closeable

sealed class CseManipulator(
        protected var manipulatorPtr: ManipulatorPtr
) : Closeable {

    override fun close() {
        if (manipulatorPtr != 0L) {
            CseLibrary.destroyManipulator(manipulatorPtr)
            manipulatorPtr = 0L
        }
    }

}

class CseOverrideManipulator(
        manipulator: ManipulatorPtr
) : CseManipulator(manipulator) {

    fun setReal(slaveIndex: Int, vr: Long, value: Double)
            = CseLibrary.setReal(manipulatorPtr, slaveIndex, longArrayOf(vr), doubleArrayOf(value))

    fun setReal(slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setReal(manipulatorPtr, slaveIndex, vr, values)
    }

    fun setInteger(slaveIndex: Int, vr: Long, value: Int)
            = CseLibrary.setInteger(manipulatorPtr, slaveIndex, longArrayOf(vr), intArrayOf(value))

    fun setInteger(slaveIndex: Int, vr: LongArray, values: IntArray): Boolean {
        return CseLibrary.setInteger(manipulatorPtr, slaveIndex, vr, values)
    }

    fun setBoolean(slaveIndex: Int, vr: Long, value: Boolean)
            = CseLibrary.setBoolean(manipulatorPtr, slaveIndex, longArrayOf(vr), booleanArrayOf(value))

    fun setBoolean(slaveIndex: Int, vr: LongArray, values: BooleanArray): Boolean {
        return CseLibrary.setBoolean(manipulatorPtr, slaveIndex, vr, values)
    }

    fun setString(slave: CseSlave, vr: Long, value: String)
            = setString(slave.index, vr, value)

    fun setString(slaveIndex: Int, vr: Long, value: String)
            = CseLibrary.setString(manipulatorPtr, slaveIndex, longArrayOf(vr), arrayOf(value))

    fun setString(slaveIndex: Int, vr: LongArray, values: Array<String>): Boolean {
        return CseLibrary.setString(manipulatorPtr, slaveIndex, vr, values)
    }

}

class CseScenario(
        manipulator: ManipulatorPtr
) : CseManipulator(manipulator) {

    fun isRunning(): Boolean {
        return CseLibrary.isScenarioRunning(manipulatorPtr)
    }

    fun abort(): Boolean {
        return CseLibrary.abortScenario(manipulatorPtr)
    }

}
