package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ObserverPtr
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File

sealed class CseObserver(
        protected var observerPtr: ObserverPtr
) : Closeable {

    override fun close() {
        if (observerPtr != 0L) {
            CseLibrary.destroyObserver(observerPtr)
            observerPtr = 0L
        }
    }

}

interface StepEventListener {
    fun post()
}

class CseStepEventListener(
        observer: ObserverPtr
): CseObserver(observer)

class CseFileObserver(
        observer: ObserverPtr,
        val logDir: File
) : CseObserver(observer)

class CseTimeSeriesObserver(
        observer: ObserverPtr
) : CseObserver(observer) {

    fun startObserving(slaveIndex: Int, variable: CseVariableDescription): Boolean {
        return CseLibrary.startObserving(observerPtr, slaveIndex, variable.type, variable.valueReference)
    }

    fun stopObserving(slaveIndex: Int, variable: CseVariableDescription): Boolean {
        return CseLibrary.stopObserving(observerPtr, slaveIndex, variable.type, variable.valueReference)
    }

    fun getRealSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return CseLibrary.getRealSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples)
    }

    fun getIntegerSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return CseLibrary.getIntegerSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples)
    }

    fun getStepNumbersForDuration(slaveIndex: Int, duration: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CseLibrary.getStepNumbersForDuration(observerPtr, slaveIndex, duration, steps)
            steps[0] to steps[1]
        }
    }

    fun getStepNumbers(slaveIndex: Int, begin: Double, end: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CseLibrary.getStepNumbers(observerPtr, slaveIndex, begin, end, steps)
            steps[0] to steps[1]
        }
    }

}

class CseLastValueObserver(
        observer: ObserverPtr
) : CseObserver(observer) {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CseLastValueObserver::class.java)

    }

    fun getReal(slaveIndex: Int, vr: Long): Double? {
        return DoubleArray(1).also {
            if (!getReal(slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getReal(slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean {
        return CseLibrary.getReal(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getReal failed, last reported error: ${CseLibrary.getLastError()}")
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
        return CseLibrary.getInteger(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getInteger failed, last reported error: ${CseLibrary.getLastError()}")
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
        return CseLibrary.getBoolean(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getBoolean failed, last reported error: ${CseLibrary.getLastError()}")
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
        return CseLibrary.getString(observerPtr, slaveIndex, vr, ref).also {
            if (!it) {
                LOG.warn("getString failed, last reported error: ${CseLibrary.getLastError()}")
            }
        }
    }

}
