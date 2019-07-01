package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_manipulator
import java.io.Closeable


sealed class CseManipulator(
        protected val manipulator: cse_manipulator
) : Closeable {

    fun setReal(slave: CseSlave, vr: LongArray, values: DoubleArray): Boolean {
        return CseLibrary.setReal(manipulator, slave.index, vr, values)
    }

    fun setInteger(slave: CseSlave, vr: LongArray, values: IntArray): Boolean {
        return CseLibrary.setInteger(manipulator, slave.index, vr, values)
    }

    override fun close() {
        CseLibrary.destroyManipulator(manipulator)
    }

}

class CseScenario(
        manipulator: cse_manipulator
) : CseManipulator(manipulator) {

    fun isRunning(): Boolean {
        return CseLibrary.isScenarioRunning(manipulator)
    }

    fun abort(): Boolean {
        return CseLibrary.abortScenario(manipulator)
    }

}
