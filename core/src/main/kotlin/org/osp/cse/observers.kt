package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_observer
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File
import java.nio.ByteBuffer
import java.nio.ByteOrder

sealed class CseObserver(
    protected var observer: cse_observer
) : Closeable {

    fun getReal(slave: CseSlave, vr: LongArray, ref: DoubleArray): Boolean {
        return CseLibrary.getReal(observer, slave.index, vr, ref)
    }

    fun getInteger(slave: CseSlave, vr: LongArray, ref: IntArray): Boolean {
        return CseLibrary.getInteger(observer, slave.index, vr, ref)
    }

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
        CseLibrary.destroyObserver(observer).also { success ->
            LOG.debug("Destroyed instance of ${javaClass.simpleName} successfully: $success")
        }
    }

    private companion object {

        private val LOG: Logger = LoggerFactory.getLogger(CseObserver::class.java)

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
