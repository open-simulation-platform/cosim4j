package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ObserverPtr
import java.io.Closeable
import java.io.File

sealed class CseObserver(
        protected var observerPtr: ObserverPtr
) : Closeable {

    fun getStepNumbersForDuration(slave: CseSlave, duration: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CseLibrary.getStepNumbersForDuration(observerPtr, slave.index, duration, steps)
            steps[0] to steps[1]
        }
    }

    fun getStepNumbers(slave: CseSlave, begin: Double, end: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CseLibrary.getStepNumbers(observerPtr, slave.index, begin, end, steps)
            steps[0] to steps[1]
        }
    }

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

    fun getReal(slave: CseSlave, vr: Long) = getReal(slave.index, vr)

    fun getReal(slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean {
        return CseLibrary.getReal(observerPtr, slaveIndex, vr, ref)
    }

    fun getReal(slave: CseSlave, vr: LongArray, ref: DoubleArray) = getReal(slave.index, vr, ref)

    fun getInteger(slaveIndex: Int, vr: Long): Int? {
        return IntArray(1).also {
            if (!CseLibrary.getInteger(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getInteger(slave: CseSlave, vr: Long) = getInteger(slave.index, vr)

    fun getInteger(slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean {
        return CseLibrary.getInteger(observerPtr, slaveIndex, vr, ref)
    }

    fun getInteger(slave: CseSlave, vr: LongArray, ref: IntArray) = getInteger(slave.index, vr, ref)

    fun getBoolean(slaveIndex: Int, vr: Long): Boolean? {
        return BooleanArray(1).also {
            if (!CseLibrary.getBoolean(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getBoolean(slave: CseSlave, vr: Long) = getBoolean(slave.index, vr)

    fun getBoolean(slaveIndex: Int, vr: LongArray, ref: BooleanArray): Boolean {
        return CseLibrary.getBoolean(observerPtr, slaveIndex, vr, ref)
    }

    fun getBoolean(slave: CseSlave, vr: LongArray, ref: BooleanArray) = getBoolean(slave.index, vr, ref)

    fun getString(slaveIndex: Int, vr: Long): String? {
        return Array<String>(1) { "" }.also {
            if (!CseLibrary.getString(observerPtr, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getString(slave: CseSlave, vr: Long) = getString(slave.index, vr)

    fun getString(slaveIndex: Int, vr: LongArray, ref: Array<String>): Boolean {
        return CseLibrary.getString(observerPtr, slaveIndex, vr, ref)
    }

    fun getString(slave: CseSlave, vr: LongArray, ref: Array<String>) = getString(slave.index, vr, ref)


    fun getRealSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return CseRealSamples().also {
            CseLibrary.getRealSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples, it)
        }
    }

    fun getRealSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int) = getRealSamples(slave.index, vr, stepNumber, nSamples)

    fun getIntegerSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return CseIntegerSamples().also {
            CseLibrary.getIntegerSamples(observerPtr, slaveIndex, vr, stepNumber, nSamples, it)
        }
    }

    fun getIntegerSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int) = getIntegerSamples(slave.index, vr, stepNumber, nSamples)

}
