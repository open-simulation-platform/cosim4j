package org.osp.cse

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.util.*


sealed class CseSamplesDirect(
    bufferSize: Int
) {

    val size: Int = -1

    private val stepBuffer = ByteBuffer.allocateDirect(bufferSize*8)

    val steps: LongArray by lazy {
        stepBuffer.position(0)
        stepBuffer.order(ByteOrder.LITTLE_ENDIAN)
        LongArray(size).also {
            stepBuffer.asLongBuffer().get(it)
        }
    }

    private val timeBuffer = ByteBuffer.allocateDirect(bufferSize*8)

    val times: DoubleArray by lazy {
        timeBuffer.position(0)
        timeBuffer.order(ByteOrder.LITTLE_ENDIAN)
        DoubleArray(size).also {
            timeBuffer.asDoubleBuffer().get(it)
        }
    }

}

class CseRealSamplesDirect(
    bufferSize: Int
): CseSamplesDirect(bufferSize) {

    private val valueBuffer = ByteBuffer.allocateDirect(bufferSize*8)

    val values: DoubleArray by lazy {
        valueBuffer.position(0)
        valueBuffer.order(ByteOrder.LITTLE_ENDIAN)
        DoubleArray(size).also {
            valueBuffer.asDoubleBuffer().get(it)
        }
    }

    override fun toString(): String {
        return "CseRealSamplesDirect(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}

class CseIntegerSamplesDirect(
    bufferSize: Int
): CseSamplesDirect(bufferSize) {

    private val valueBuffer= ByteBuffer.allocateDirect(bufferSize*4)

    val values: IntArray by lazy {
        valueBuffer.position(0)
        valueBuffer.order(ByteOrder.LITTLE_ENDIAN)
        IntArray(size).also {
            valueBuffer.asIntBuffer().get(it)
        }
    }

    override fun toString(): String {
        return "CseIntegerSamplesDirect(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}