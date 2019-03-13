package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_observer
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.nio.ByteBuffer
import java.nio.ByteOrder

sealed class CseObserver(
    private var observer_: cse_observer?
) : Closeable {

    protected val observer: cse_observer =
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
    observer: cse_observer
) : CseObserver(observer)

class CseMembufferObserver(
    observer: cse_observer
) : CseObserver(observer) {

    fun getReal(slave: CseSlave, vr: Long): Double {
        return DoubleArray(1).also {
            getReal(slave, longArrayOf(vr), it)
        }[0]
    }

    fun getRealDirect(slave: CseSlave, vr: Long): Double {
        return DoubleArray(1).also {
            getRealDirect(slave, longArrayOf(vr), it)
        }[0]
    }

    fun getReal(slave: CseSlave, vr: LongArray, ref: DoubleArray): Boolean {
        return CseLibrary.getReal(observer, slave.index, vr, ref)
    }

    fun getRealDirect(slave: CseSlave, vr: LongArray, ref: DoubleArray): Boolean {
        val vrBuf = ByteBuffer.allocateDirect(vr.size * 8).apply {
            order(ByteOrder.LITTLE_ENDIAN)
            asLongBuffer().put(vr)
            rewind()
        }
        val refBuf = ByteBuffer.allocateDirect(vr.size * 8)
        return CseLibrary.getRealDirect(observer, slave.index, vrBuf, vr.size, refBuf).also {
            refBuf.order(ByteOrder.LITTLE_ENDIAN)
            refBuf.position(0)
            refBuf.asDoubleBuffer().get(ref)
        }
    }

    fun getRealSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return CseRealSamples().also {
            CseLibrary.getRealSamples(observer, slave.index, vr, stepNumber, nSamples, it)
        }
    }

    fun getRealSamplesDirect(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamplesDirect {
        return CseRealSamplesDirect(nSamples).also {
            CseLibrary.getRealSamplesDirect(observer, slave.index, vr, stepNumber, nSamples, it)
        }
    }

    fun getInteger(slave: CseSlave, vr: Long): Int {
        val ref = IntArray(1)
        return getInteger(slave, longArrayOf(vr), ref).let { ref[0] }
    }

    fun getInteger(slave: CseSlave, vr: LongArray, ref: IntArray): Boolean {
        return CseLibrary.getInteger(observer, slave.index, vr, ref)
    }

    fun getIntegerDirect(slave: CseSlave, vr: LongArray, ref: IntArray): Boolean {
        val vrBuf = ByteBuffer.allocateDirect(vr.size * 8).apply {
            order(ByteOrder.LITTLE_ENDIAN)
            asLongBuffer().put(vr)
            rewind()
        }
        val refBuf = ByteBuffer.allocateDirect(vr.size * 5)
        return CseLibrary.getRealDirect(observer, slave.index, vrBuf, vr.size, refBuf).also {
            refBuf.order(ByteOrder.LITTLE_ENDIAN)
            refBuf.position(0)
            refBuf.asIntBuffer().get(ref)
        }
    }

    fun getIntegerSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return CseIntegerSamples().also {
            CseLibrary.getIntegerSamples(observer, slave.index, vr, stepNumber, nSamples, it)
        }
    }

    fun getIntegerSamplesDirect(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamplesDirect {
        return CseIntegerSamplesDirect(nSamples).also {
            CseLibrary.getIntegerSamplesDirect(observer, slave.index, vr, stepNumber, nSamples, it)
        }
    }

}