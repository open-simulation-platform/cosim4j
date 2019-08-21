package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_manipulator
import java.io.Closeable

sealed class CseManipulator(
        protected var manipulator: cse_manipulator
) : Closeable {

    override fun close() {
        if (manipulator != 0L) {
            CseLibrary.destroyManipulator(manipulator)
            manipulator = 0L
        }
    }

}

class CseOverrideManipulator(
        manipulator: cse_manipulator
) : CseManipulator(manipulator) {

    fun setReal(slave: CseSlave, vr: Long, value: Double)
            = setReal(slave.index, vr, value)

    fun setReal(slaveIndex: Int, vr: Long, value: Double)
            = CseLibrary.setReal(manipulator, slaveIndex, longArrayOf(vr), doubleArrayOf(value))

    fun setReal(slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setReal(manipulator, slaveIndex, vr, values)
    }

    fun setReal(slave: CseSlave, vr: LongArray, values: DoubleArray): Boolean {
        return setReal(slave.index, vr, values)
    }

    fun setInteger(slave: CseSlave, vr: Long, value: Int)
            = setInteger(slave.index, vr, value)

    fun setInteger(slaveIndex: Int, vr: Long, value: Int)
            = CseLibrary.setInteger(manipulator, slaveIndex, longArrayOf(vr), intArrayOf(value))

    fun setInteger(slaveIndex: Int, vr: LongArray, values: IntArray): Boolean {
        return CseLibrary.setInteger(manipulator, slaveIndex, vr, values)
    }

    fun setInteger(slave: CseSlave, vr: LongArray, values: IntArray): Boolean {
        return setInteger(slave.index, vr, values)
    }

    fun setBoolean(slave: CseSlave, vr: Long, value: Boolean)
            = setBoolean(slave.index, vr, value)

    fun setBoolean(slaveIndex: Int, vr: Long, value: Boolean)
            = CseLibrary.setBoolean(manipulator, slaveIndex, longArrayOf(vr), booleanArrayOf(value))

    fun setBoolean(slaveIndex: Int, vr: LongArray, values: BooleanArray): Boolean {
        return CseLibrary.setBoolean(manipulator, slaveIndex, vr, values)
    }

    fun setBoolean(slave: CseSlave, vr: LongArray, values: BooleanArray): Boolean {
        return setBoolean(slave.index, vr, values)
    }

    fun setString(slave: CseSlave, vr: Long, value: String)
            = setString(slave.index, vr, value)

    fun setString(slaveIndex: Int, vr: Long, value: String)
            = CseLibrary.setString(manipulator, slaveIndex, longArrayOf(vr), arrayOf(value))

    fun setString(slaveIndex: Int, vr: LongArray, values: Array<String>): Boolean {
        return CseLibrary.setString(manipulator, slaveIndex, vr, values)
    }

    fun setString(slave: CseSlave, vr: LongArray, values: Array<String>): Boolean {
        return setString(slave.index, vr, values)
    }

}

class CseScenario(
        manipulator: cse_manipulator
) : CseManipulator(manipulator) {

    fun isRunning(): Boolean {
        return CseLibrary.isScenarioRunning(manipulator)
    }

    fun abort(): Boolean {
        return CseLibrary.abortScenario(manipulator)
    }

}
