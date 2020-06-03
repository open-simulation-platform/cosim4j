package org.osp.cosim

sealed class CosimSamples(
        val size: Int,
        val steps: LongArray,
        val times: DoubleArray
)

class CosimRealSamples(
        size: Int,
        steps: LongArray,
        times: DoubleArray,
        val values: DoubleArray
) : CosimSamples(size, steps, times) {

    override fun toString(): String {
        return "CosimRealSamples(size=$size\nvalues=${values.contentToString()}\nsteps=${steps.contentToString()}\ntimes=${times.contentToString()})"
    }

}

class CosimIntegerSamples(
        size: Int,
        steps: LongArray,
        times: DoubleArray,
        val values: IntArray
) : CosimSamples(size, steps, times) {

    override fun toString(): String {
        return "CosimIntegerSamples(size=$size\nvalues=${values.contentToString()}\nsteps=${steps.contentToString()}\ntimes=${times.contentToString()})"
    }

}
