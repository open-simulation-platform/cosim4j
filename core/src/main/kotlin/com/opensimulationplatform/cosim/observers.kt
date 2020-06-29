package com.opensimulationplatform.cosim

import com.opensimulationplatform.cosim.jni.CosimLibrary
import com.opensimulationplatform.cosim.jni.ObserverPtr
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File

sealed class CosimObserver(
    protected var observerPtr: ObserverPtr
) : Closeable {

    override fun close() {
        if (observerPtr != 0L) {
            CosimLibrary.destroyObserver(observerPtr)
            observerPtr = 0L
        }
    }

}

interface StepEventListener {
    fun addedToExecution(execution: CosimExecution)
    fun onSimulationInitialized(currentTime: Double)
    fun onStepComplete(stepNumber: Long, currentTime: Double, lastStepSize: Double)
}

abstract class StepEventAdapter : StepEventListener {
    override fun addedToExecution(execution: CosimExecution) {}
    override fun onSimulationInitialized(currentTime: Double) {}
    override fun onStepComplete(stepNumber: Long, currentTime: Double, lastStepSize: Double) {}
}

class CosimStepEventListener internal constructor(
    observer: ObserverPtr
) : CosimObserver(observer)

class CosimFileObserver internal constructor(
    observer: ObserverPtr,
    val logDir: File
) : CosimObserver(observer)

class CosimTimeSeriesObserver internal constructor(
    observer: ObserverPtr
) : CosimObserver(observer) {

    fun startObserving(slaveIndex: Int, variable: CosimVariableDescription): Boolean {
        return CosimLibrary.startObserving(observerPtr, slaveIndex, variable.type, variable.valueReference)
    }

    fun stopObserving(slaveIndex: Int, variable: CosimVariableDescription): Boolean {
        return CosimLibrary.stopObserving(observerPtr, slaveIndex, variable.type, variable.valueReference)
    }

    fun getRealSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CosimRealSamples {
        return CosimLibrary.getRealSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples)
    }

    fun getIntegerSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CosimIntegerSamples {
        return CosimLibrary.getIntegerSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples)
    }

    fun getStepNumbersForDuration(slaveIndex: Int, duration: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CosimLibrary.getStepNumbersForDuration(observerPtr, slaveIndex, duration, steps)
            steps[0] to steps[1]
        }
    }

    fun getStepNumbers(slaveIndex: Int, begin: Double, end: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CosimLibrary.getStepNumbers(observerPtr, slaveIndex, begin, end, steps)
            steps[0] to steps[1]
        }
    }

}

class CosimLastValueObserver(
    observer: ObserverPtr
) : CosimObserver(observer) {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CosimLastValueObserver::class.java)

    }

    fun getReal(slaveIndex: Int, vr: Long): Double? {
        return DoubleArray(1).also {
            if (!getReal(slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getReal(slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean {
        return CosimLibrary.getReal(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getReal failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

    fun getInteger(slaveIndex: Int, vr: Long): Int? {
        return IntArray(1).also {
            if (!getInteger(slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getInteger(slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean {
        return CosimLibrary.getInteger(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getInteger failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

    fun getBoolean(slaveIndex: Int, vr: Long): Boolean? {
        return BooleanArray(1).also {
            if (!getBoolean(slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getBoolean(slaveIndex: Int, vr: LongArray, ref: BooleanArray): Boolean {
        return CosimLibrary.getBoolean(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getBoolean failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

    fun getString(slaveIndex: Int, vr: Long): String? {
        return Array(1) { "" }.also {
            if (!getString(slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getString(slaveIndex: Int, vr: LongArray, ref: Array<String>): Boolean {
        return CosimLibrary.getString(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getString failed, last reported error: ${CosimLibrary.getLastError()}")
            }
        }
    }

}
