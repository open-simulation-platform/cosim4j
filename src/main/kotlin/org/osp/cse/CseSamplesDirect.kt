package org.osp.cse

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.util.*


sealed class CseSamplesDirect {

    val size: Int = -1

    private lateinit var stepBuffer: ByteBuffer
        private set

    val steps: LongArray by lazy {
        stepBuffer.position(0)
        stepBuffer.order(ByteOrder.LITTLE_ENDIAN)
        LongArray(stepBuffer.capacity()/Long.SIZE_BYTES).also {
            stepBuffer.asLongBuffer().get(it)
        }
    }

    private lateinit var timeBuffer: ByteBuffer
        private set

    val times: DoubleArray by lazy {
        timeBuffer.position(0)
        timeBuffer.order(ByteOrder.LITTLE_ENDIAN)
        DoubleArray(timeBuffer.capacity()/8).also {
            timeBuffer.asDoubleBuffer().get(it)
        }
    }

}

class CseRealSamplesDirect: CseSamplesDirect() {

    private lateinit var valueBuffer: ByteBuffer
        private set

    val values: DoubleArray by lazy {
        valueBuffer.position(0)
        valueBuffer.order(ByteOrder.LITTLE_ENDIAN)
        DoubleArray(valueBuffer.capacity()/8).also {
            valueBuffer.asDoubleBuffer().get(it)
        }
    }

    override fun toString(): String {
        return "CseRealSamples(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}

class CseIntegerSamplesDirect: CseSamplesDirect() {

    lateinit var values: IntArray
        private set

    override fun toString(): String {
        return "CseIntegerSamples(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}