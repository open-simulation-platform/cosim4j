package com.opensimulationplatform.cosim

import com.opensimulationplatform.cosim.jni.CosimLibrary
import com.opensimulationplatform.cosim.jni.ManipulatorPtr
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable

sealed class CosimManipulator(
    protected var manipulatorPtr: ManipulatorPtr
) : Closeable {

    override fun close() {
        if (manipulatorPtr != 0L) {
            CosimLibrary.destroyManipulator(manipulatorPtr)
            manipulatorPtr = 0L
        }
    }

}

class CosimOverrideManipulator(
    manipulator: ManipulatorPtr
) : CosimManipulator(manipulator) {

    private companion object {
        val LOG: Logger = LoggerFactory.getLogger(CosimOverrideManipulator::class.java)
    }

    fun setReal(slaveIndex: Int, vr: Long, value: Double): Boolean {
        return setReal(slaveIndex, longArrayOf(vr), doubleArrayOf(value))
    }

    fun setReal(slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean {
        return CosimLibrary.setReal(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setReal failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

    fun setInteger(slaveIndex: Int, vr: Long, value: Int): Boolean {
        return setInteger(slaveIndex, longArrayOf(vr), intArrayOf(value))
    }

    fun setInteger(slaveIndex: Int, vr: LongArray, values: IntArray): Boolean {
        return CosimLibrary.setInteger(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setInteger failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

    fun setBoolean(slaveIndex: Int, vr: Long, value: Boolean): Boolean {
        return setBoolean(slaveIndex, longArrayOf(vr), booleanArrayOf(value))
    }

    fun setBoolean(slaveIndex: Int, vr: LongArray, values: BooleanArray): Boolean {
        return CosimLibrary.setBoolean(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setBoolean failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

    fun setString(slaveIndex: Int, vr: Long, value: String): Boolean {
        return setString(slaveIndex, longArrayOf(vr), arrayOf(value))
    }

    fun setString(slaveIndex: Int, vr: LongArray, values: Array<String>): Boolean {
        return CosimLibrary.setString(manipulatorPtr, slaveIndex, vr, values).also {
            if (!it) {
                LOG.warn("setString failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

}

class CosimScenario(
    manipulator: ManipulatorPtr
) : CosimManipulator(manipulator) {

    fun isRunning(): Boolean {
        return CosimLibrary.isScenarioRunning(manipulatorPtr)
    }

    fun abort(): Boolean {
        return CosimLibrary.abortScenario(manipulatorPtr)
    }

}
