package org.osp.cse

sealed class CseSamples {

    val size: Int = -1

    lateinit var steps: LongArray
        private set

    lateinit var times: DoubleArray
        private set

}

class CseRealSamples : CseSamples() {

    lateinit var values: DoubleArray
        private set

    override fun toString(): String {
        return "CseRealSamples(size=$size\nvalues=${values.contentToString()}\nsteps=${steps.contentToString()}\ntimes=${times.contentToString()})"
    }

}

class CseIntegerSamples : CseSamples() {

    lateinit var values: IntArray
        private set

    override fun toString(): String {
        return "CseIntegerSamples(size=$size\nvalues=${values.contentToString()}\nsteps=${steps.contentToString()}\ntimes=${times.contentToString()})"
    }

}
