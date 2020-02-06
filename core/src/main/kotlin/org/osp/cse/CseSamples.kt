package org.osp.cse

sealed class CseSamples(
        val size: Int,
        val steps: LongArray,
        val times: DoubleArray
)

class CseRealSamples(
        size: Int,
        steps: LongArray,
        times: DoubleArray,
        val values: DoubleArray
) : CseSamples(size, steps, times) {

    override fun toString(): String {
        return "CseRealSamples(size=$size\nvalues=${values.contentToString()}\nsteps=${steps.contentToString()}\ntimes=${times.contentToString()})"
    }

}

class CseIntegerSamples(
        size: Int,
        steps: LongArray,
        times: DoubleArray,
        val values: IntArray
) : CseSamples(size, steps, times) {

    override fun toString(): String {
        return "CseIntegerSamples(size=$size\nvalues=${values.contentToString()}\nsteps=${steps.contentToString()}\ntimes=${times.contentToString()})"
    }

}
