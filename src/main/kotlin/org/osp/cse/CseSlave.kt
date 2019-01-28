package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_slave
import java.io.File
import java.nio.ByteBuffer
import java.nio.ByteOrder
import kotlin.math.asin

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
        return CseLibrary.setRealDirect(execution, index, longBuffer(vr), vr.size, intBuffer(values))
    }

    fun setReal(vr: Long, value: Double): Boolean {
        return setReal(longArrayOf(vr), doubleArrayOf(value))
    }

    fun setReal(vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setReal(execution, index, vr, values)
    }

    fun setRealDirect(vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setRealDirect(execution, index, longBuffer(vr), vr.size, doubleBuffer(values))
    }

    private companion object {

        private fun longBuffer(longs: LongArray) : ByteBuffer {
            return ByteBuffer.allocateDirect(longs.size*8).apply {
                order(ByteOrder.LITTLE_ENDIAN)
                asLongBuffer().put(longs)
                rewind()
            }
        }

        private fun doubleBuffer(doubles: DoubleArray) : ByteBuffer {
            return ByteBuffer.allocateDirect(doubles.size*8).apply {
                order(ByteOrder.LITTLE_ENDIAN)
                asDoubleBuffer().put(doubles)
                rewind()
            }
        }

        private fun intBuffer(ints: IntArray) : ByteBuffer {
            return ByteBuffer.allocateDirect(ints.size*4).apply {
                order(ByteOrder.LITTLE_ENDIAN)
                asIntBuffer().put(ints)
                rewind()
            }
        }

    }

}