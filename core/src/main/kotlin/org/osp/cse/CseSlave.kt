package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_slave
import java.io.Closeable

class CseSlave internal constructor(
        val index: Int,
        private var slave: cse_slave,
        private val execution: cse_execution
) : Closeable {


    val variables: Array<CseVariableDescription> by lazy {
        Array(CseLibrary.getNumVariables(execution, index)) { CseVariableDescription() }.also {
            CseLibrary.getVariables(execution, index, it)
        }
    }

    fun getReal(observer: CseLastValueObserver, vr: Long): Double? {
        return observer.getReal(this, vr)
    }

    fun getReal(observer: CseLastValueObserver, vr: LongArray, ref: DoubleArray): Boolean {
        return observer.getReal(this, vr, ref)
    }

    fun getInteger(observer: CseLastValueObserver, vr: Long): Int? {
        return observer.getInteger(this, vr)
    }

    fun getInteger(observer: CseLastValueObserver, vr: LongArray, ref: IntArray): Boolean {
        return observer.getInteger(this, vr, ref)
    }

    fun setReal(manipulator: CseManipulator, vr: LongArray, values: DoubleArray): Boolean {
        return manipulator.setReal(this, vr, values)
    }

    fun setInteger(manipulator: CseManipulator, vr: LongArray, values: IntArray): Boolean {
        return manipulator.setInteger(this, vr, values)
    }

    fun getRealSamples(observer: CseLastValueObserver, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
        return observer.getRealSamples(this, vr, stepNumber, nSamples)
    }

    fun getIntegerSamples(observer: CseLastValueObserver, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
        return observer.getIntegerSamples(this, vr, stepNumber, nSamples)
    }

    override fun close() {
        if (slave != 0L) {
            CseLibrary.destroySlave(slave)
            slave = 0L
        }
    }

}

class CseSlaveInfo {

    lateinit var name: String
        private set

    lateinit var source: String
        private set

    var index: Int = -1
        private set

    override fun toString(): String {
        return "CseSlaveInfo(name=$name, source=$source, index=$index)"
    }

}
