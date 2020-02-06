package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ManipulatorPtr
import org.slf4j.Logger
import org.slf4j.LoggerFactory
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

    private companion object {
        val LOG: Logger = LoggerFactory.getLogger(CseOverrideManipulator::class.java)
    }

    fun setReal(slaveIndex: Int, vr: Long, value: Double): Boolean {
        return setReal(slaveIndex, longArrayOf(vr), doubleArrayOf(value))
    }

    fun setReal(slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setReal(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setReal failed, last reported error: ${CseLibrary.getLastError()}")
            }
        }
    }

    fun setInteger(slaveIndex: Int, vr: Long, value: Int): Boolean {
        return setInteger(slaveIndex, longArrayOf(vr), intArrayOf(value))
    }

    fun setInteger(slaveIndex: Int, vr: LongArray, values: IntArray): Boolean {
        return CseLibrary.setInteger(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setInteger failed, last reported error: ${CseLibrary.getLastError()}")
            }
        }
    }

    fun setBoolean(slaveIndex: Int, vr: Long, value: Boolean): Boolean {
        return setBoolean(slaveIndex, longArrayOf(vr), booleanArrayOf(value))
    }

    fun setBoolean(slaveIndex: Int, vr: LongArray, values: BooleanArray): Boolean {
        return CseLibrary.setBoolean(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setBoolean failed, last reported error: ${CseLibrary.getLastError()}")
            }
        }
    }

    fun setString(slaveIndex: Int, vr: Long, value: String): Boolean {
        return setString(slaveIndex, longArrayOf(vr), arrayOf(value))
    }

    fun setString(slaveIndex: Int, vr: LongArray, values: Array<String>): Boolean {
        return CseLibrary.setString(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setString failed, last reported error: ${CseLibrary.getLastError()}")
            }
        }
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
