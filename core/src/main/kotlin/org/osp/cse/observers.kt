package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_observer
import java.io.Closeable
import java.io.File

sealed class CseObserver(
    protected var observer: cse_observer
) : Closeable {

    fun getStepNumbersForDuration(slave: CseSlave, duration: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CseLibrary.getStepNumbersForDuration(observer, slave.index, duration, steps)
            steps[0] to steps[1]
        }
    }

    fun getStepNumbers(slave: CseSlave, begin: Double, end: Double): Pair<Long, Long> {
        return LongArray(2).let { steps ->
            CseLibrary.getStepNumbers(observer, slave.index, begin, end, steps)
            steps[0] to steps[1]
        }
    }

    override fun close() {
        if (observer != 0L) {
            CseLibrary.destroyObserver(observer)
            observer = 0L
        }
    }

}

class CseFileObserver(
    observer: cse_observer,
    val logDir: File
) : CseObserver(observer)

class CseTimeSeriesObserver(
        observer: cse_observer
) : CseObserver(observer) {

    fun startObserver(): Boolean {
        return CseLibrary.startObserving(observer)
    }

    fun stopObserving(): Boolean {
        return CseLibrary.stopObserving(observer)
    }

}

class CseLastValueObserver(
        observer: cse_observer
): CseObserver(observer) {

    fun getReal(slaveIndex: Int, vr: Long): Double? {
        return DoubleArray(1).also {
            if (!CseLibrary.getReal(observer, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getReal(slave: CseSlave, vr: Long) = getReal(slave.index, vr)

    fun getReal(slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean {
        return CseLibrary.getReal(observer, slaveIndex, vr, ref)
    }

    fun getReal(slave: CseSlave, vr: LongArray, ref: DoubleArray) = getReal(slave.index, vr, ref)

    fun getInteger(slaveIndex: Int, vr: Long): Int? {
        return IntArray(1).also {
            if (!CseLibrary.getInteger(observer, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getInteger(slave: CseSlave, vr: Long) = getInteger(slave.index, vr)

    fun getInteger(slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean {
        return CseLibrary.getInteger(observer, slaveIndex, vr, ref)
    }

    fun getInteger(slave: CseSlave, vr: LongArray, ref: IntArray) = getInteger(slave.index, vr, ref)

    fun getBoolean(slaveIndex: Int, vr: Long): Boolean? {
        return BooleanArray(1).also {
            if (!CseLibrary.getBoolean(observer, slaveIndex, longArrayOf(vr), it)) {
                return null
            }
        }[0]
    }

    fun getBoolean(slave: CseSlave, vr: Long) = getBoolean(slave.index, vr)

    fun getBoolean(slaveIndex: Int, vr: LongArray, ref: BooleanArray): Boolean {
        return CseLibrary.getBoolean(observer, slaveIndex, vr, ref)
    }

    fun getBoolean(slave: CseSlave, vr: LongArray, ref: BooleanArray) = getBoolean(slave.index, vr, ref)

    fun getRealSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return CseRealSamples().also {
            CseLibrary.getRealSamples(observer, slaveIndex, vr, stepNumber, nSamples, it)
        }
    }

    fun getRealSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int) = getRealSamples(slave.index, vr, stepNumber, nSamples)

    fun getIntegerSamples(slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return CseIntegerSamples().also {
            CseLibrary.getIntegerSamples(observer, slaveIndex, vr, stepNumber, nSamples, it)
        }
    }

    fun getIntegerSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int) = getIntegerSamples(slave.index, vr, stepNumber, nSamples)

}
