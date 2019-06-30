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
    private var observer_: cse_observer?
) : Closeable {

    internal val observer: cse_observer =
        observer_ ?: throw IllegalStateException("${javaClass.simpleName} has been closed!")

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
        observer_?.also {
            CseLibrary.destroyObserver(it).also { success ->
                observer_ = null
                LOG.debug("Destroyed instance of ${javaClass.simpleName} successfully: $success")
            }
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

    fun stopObservering(): Boolean {
        return CseLibrary.stopObserving(observer)
    }

}
