package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ObserverPtr
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

    fun startObserver(): Boolean {
        return CseLibrary.startObserving(observerPtr)
    }

    fun stopObserving(): Boolean {
        return CseLibrary.stopObserving(observerPtr)
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

    fun getReal(slaveIndex: Int, vr: Long): Double? {
        return DoubleArray(1).also {
            if (!CseLibrary.getReal(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getReal(slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean {
        return CseLibrary.getReal(observerPtr, slaveIndex, vr, ref)
    }

    fun getInteger(slaveIndex: Int, vr: Long): Int? {
        return IntArray(1).also {
            if (!CseLibrary.getInteger(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getInteger(slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean {
        return CseLibrary.getInteger(observerPtr, slaveIndex, vr, ref)
    }

    fun getBoolean(slaveIndex: Int, vr: Long): Boolean? {
        return BooleanArray(1).also {
            if (!CseLibrary.getBoolean(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getBoolean(slaveIndex: Int, vr: LongArray, ref: BooleanArray): Boolean {
        return CseLibrary.getBoolean(observerPtr, slaveIndex, vr, ref)
    }

    fun getString(slaveIndex: Int, vr: Long): String? {
        return Array<String>(1) { "" }.also {
            if (!CseLibrary.getString(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getString(slaveIndex: Int, vr: LongArray, ref: Array<String>): Boolean {
        return CseLibrary.getString(observerPtr, slaveIndex, vr, ref)
    }

    fun getRealSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return CseRealSamples().also {
            CseLibrary.getRealSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples, it)
        }
    }

    fun getIntegerSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return CseIntegerSamples().also {
            CseLibrary.getIntegerSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples, it)
        }
    }

}
