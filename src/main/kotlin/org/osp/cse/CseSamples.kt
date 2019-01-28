package org.osp.cse

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.util.*


sealed class CseSamples{

    val size: Int = -1

    lateinit var steps: LongArray
        private set

    lateinit var times: DoubleArray
        private set

}

class CseRealSamples: CseSamples() {

    lateinit var values: DoubleArray
        private set

    override fun toString(): String {
        return "CseRealSamples(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}

class CseIntegerSamples: CseSamples() {

    lateinit var values: IntArray
        private set

    override fun toString(): String {
        return "CseIntegerSamples(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}


sealed class CseSamplesDirect(
    bufferSize: Int
) {

    val size: Int = -1

    private val stepBuffer = ByteBuffer.allocateDirect(bufferSize*8)

    val steps: LongArray by lazy {
        LongArray(size).also {
            stepBuffer.fill(it)
        }
    }

    private val timeBuffer = ByteBuffer.allocateDirect(bufferSize*8)

    val times: DoubleArray by lazy {
        DoubleArray(size).also {
            timeBuffer.fill(it)
        }
    }

}

class CseRealSamplesDirect(
    bufferSize: Int
): CseSamplesDirect(bufferSize) {

    private val valueBuffer = ByteBuffer.allocateDirect(bufferSize*8)

    val values: DoubleArray by lazy {
        DoubleArray(size).also {
            valueBuffer.fill(it)
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
        IntArray(size).also {
            valueBuffer.fill(it)
        }
    }

    override fun toString(): String {
        return "CseIntegerSamplesDirect(size=$size\nvalues=${Arrays.toString(values)}\nsteps=${Arrays.toString(steps)}\ntimes=${Arrays.toString(times)})"
    }

}

private fun ByteBuffer.fill(arr: IntArray) {
    this.position(0)
    this.order(ByteOrder.LITTLE_ENDIAN)
    this.asIntBuffer().get(arr)
}

private fun ByteBuffer.fill(arr: DoubleArray) {
    this.position(0)
    this.order(ByteOrder.LITTLE_ENDIAN)
    this.asDoubleBuffer().get(arr)
}

private fun ByteBuffer.fill(arr: LongArray) {
    this.position(0)
    this.order(ByteOrder.LITTLE_ENDIAN)
    this.asLongBuffer().get(arr)
}