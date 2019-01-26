package org.osp.cse

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