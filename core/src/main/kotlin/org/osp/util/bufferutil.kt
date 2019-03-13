package org.osp.util

import java.nio.ByteBuffer
import java.nio.ByteOrder


internal fun LongArray.directBuffer() : ByteBuffer {
    return ByteBuffer.allocateDirect(this.size*8).apply {
        order(ByteOrder.LITTLE_ENDIAN)
        asLongBuffer().put(this@directBuffer)
        rewind()
    }
}

internal fun DoubleArray.directBuffer() : ByteBuffer {
    return ByteBuffer.allocateDirect(this.size*8).apply {
        order(ByteOrder.LITTLE_ENDIAN)
        asDoubleBuffer().put(this@directBuffer)
        rewind()
    }
}

internal fun IntArray.directBuffer(): ByteBuffer {
    return ByteBuffer.allocateDirect(this.size*4).apply {
        order(ByteOrder.LITTLE_ENDIAN)
        asIntBuffer().put(this@directBuffer)
        rewind()
    }
}

internal fun ByteBuffer.fill(arr: IntArray) {
    this.position(0)
    this.order(ByteOrder.LITTLE_ENDIAN)
    this.asIntBuffer().get(arr)
}

internal fun ByteBuffer.fill(arr: DoubleArray) {
    this.position(0)
    this.order(ByteOrder.LITTLE_ENDIAN)
    this.asDoubleBuffer().get(arr)
}

internal fun ByteBuffer.fill(arr: LongArray) {
    this.position(0)
    this.order(ByteOrder.LITTLE_ENDIAN)
    this.asLongBuffer().get(arr)
}