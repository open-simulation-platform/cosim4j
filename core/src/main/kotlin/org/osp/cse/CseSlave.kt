package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_slave
import org.osp.util.directBuffer
import java.io.Closeable
import java.io.File

class CseSlave internal constructor(
        val index: Int,
        private val slavePointer: cse_slave
) : Closeable {

    override fun close() {
        CseLibrary.destroyLocalSlave(slavePointer)
    }
}
