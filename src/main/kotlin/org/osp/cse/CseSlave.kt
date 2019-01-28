package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_slave
import org.osp.util.directBuffer
import java.io.File

class CseSlave internal constructor(
    private val execution: cse_execution,
    fmuPath: File
) {
    private val slave: cse_slave = CseLibrary.createLocalSlave(fmuPath.absolutePath)
    val index: Int = CseLibrary.addSlave(execution, slave)

    fun setInteger(vr: Long, value: Int): Boolean {
        return setInteger(longArrayOf(vr), intArrayOf(value))
    }

    fun setInteger(vr: LongArray, values: IntArray): Boolean {
        return CseLibrary.setInteger(execution, index, vr, values)
    }

    fun setIntegerDirect(vr: LongArray, values: IntArray): Boolean {
        return CseLibrary.setRealDirect(execution, index, vr.directBuffer(), vr.size, values.directBuffer())
    }

    fun setReal(vr: Long, value: Double): Boolean {
        return setReal(longArrayOf(vr), doubleArrayOf(value))
    }

    fun setReal(vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setReal(execution, index, vr, values)
    }

    fun setRealDirect(vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setRealDirect(execution, index, vr.directBuffer(), vr.size, values.directBuffer())
    }

}