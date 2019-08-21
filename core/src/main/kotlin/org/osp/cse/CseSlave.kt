package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_slave
import java.io.Closeable

class CseSlave internal constructor(
        val index: Int,
        val name: String,
        private var slave: cse_slave,
        private val execution: cse_execution
) : Closeable {

    val variables: Array<CseVariableDescription> by lazy {
        Array(CseLibrary.getNumVariables(execution, index)) { CseVariableDescription() }.also {
            CseLibrary.getVariables(execution, index, it)
        }
    }

    fun getVariable(name: String): CseVariableDescription? {
        return variables.find { it.name == name }
    }

    fun getVariable(vr: Long, type: CseVariableType): CseVariableDescription? {
        return variables.find { it.valueReference == vr && it.type == type }
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

    fun getBoolean(observer: CseLastValueObserver, vr: Long): Boolean? {
        return observer.getBoolean(this, vr)
    }

    fun getBoolean(observer: CseLastValueObserver, vr: LongArray, ref: BooleanArray): Boolean {
        return observer.getBoolean(this, vr, ref)
    }

    fun getString(observer: CseLastValueObserver, vr: Long): String? {
        return observer.getString(this, vr)
    }

    fun getString(observer: CseLastValueObserver, vr: LongArray, ref: Array<String>): Boolean {
        return observer.getString(this, vr, ref)
    }

    fun setReal(manipulator: CseOverrideManipulator, vr: Long, value: Double): Boolean {
        return manipulator.setReal(this, vr, value)
    }

    fun setReal(manipulator: CseOverrideManipulator, vr: LongArray, values: DoubleArray): Boolean {
        return manipulator.setReal(this, vr, values)
    }

    fun setInteger(manipulator: CseOverrideManipulator, vr: Long, value: Int): Boolean {
        return manipulator.setInteger(this, vr, value)
    }

    fun setInteger(manipulator: CseOverrideManipulator, vr: LongArray, values: IntArray): Boolean {
        return manipulator.setInteger(this, vr, values)
    }

    fun setBoolean(manipulator: CseOverrideManipulator, vr: Long, value: Boolean): Boolean {
        return manipulator.setBoolean(this, vr, value)
    }

    fun setBoolean(manipulator: CseOverrideManipulator, vr: LongArray, values: BooleanArray): Boolean {
        return manipulator.setBoolean(this, vr, values)
    }

    fun setString(manipulator: CseOverrideManipulator, vr: Long, value: String): Boolean {
        return manipulator.setString(this, vr, value)
    }

    fun setString(manipulator: CseOverrideManipulator, vr: LongArray, values: Array<String>): Boolean {
        return manipulator.setString(this, vr, values)
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

    override fun toString(): String {
        return "CseSlave(name='$name', index=$index)"
    }

}

class CseSlaveInfo {

    var index: Int = -1
        private set

    lateinit var name: String
        private set

    lateinit var source: String
        private set

    override fun toString(): String {
        return "CseSlaveInfo(name=$name, index=$index, source=$source)"
    }

}
