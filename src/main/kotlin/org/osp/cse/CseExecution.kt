package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_observer
import org.osp.cse.jni.cse_slave
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File
import java.lang.IllegalStateException


class CseExecution(
    startTime: Double,
    stepSize: Double
): Closeable {

    private val cse: CseLibrary
    private val execution: cse_execution
    private var observer: cse_observer? = null

    private val status = CseExecutionStatusImpl()

    init {
        cse = CseLibrary()
        execution = cse.createExecution(startTime, stepSize)
    }

    constructor(stepSize: Double): this(0.0, stepSize)

    fun getStatus(): CseExecutionStatus {
        cse.getStatus(execution, status)
        return status
    }

    fun setMemBufferObserver() {
        observer?.apply {
            cse.destroyObserver(this)
        }
        observer = cse.createMembufferObserver().also {
            cse.addObserver(execution, it)
        }

    }

    fun setFileObserver(logDir: File) {
        observer?.apply {
            cse.destroyObserver(this)
        }
        if (logDir.exists()) {
            logDir.mkdirs()
        }
        observer = cse.createFileObserver(logDir.absoluteFile).also {
            cse.addObserver(execution, it)
        }
    }

    fun addSlave(fmuPath: File): CseSlave {
        return CseSlave(fmuPath)
    }

    fun start(): Boolean {
        return cse.start(execution)
    }

    fun step(numSteps: Long): Boolean {
        return cse.step(execution, numSteps)
    }

    fun stop(): Boolean {
        return cse.stop(execution)
    }

    fun enableRealTimeSimulation(): Boolean {
        return cse.enableRealTimeSimulation(execution)
    }

    fun disableRealTimeSimulation(): Boolean {
        return cse.disableRealTimeSimulation(execution)
    }

    fun connectIntegers(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return cse.connectIntegers(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun connectReals(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return cse.connectReals(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun getStepNumbers(slave: CseSlave, begin: Double, end: Double): Pair<Long, Long> {
        if (observer == null) {
            throw IllegalStateException("No observer has been set!")
        }
        val steps = LongArray(2)
        return cse.getStepNumbers(observer!!, slave.index, begin, end, steps).let {
            steps[0] to steps[1]
        }
    }

    override fun close() {
        observer?.apply {
            cse.destroyObserver(this).also {
                LOG.debug("Destroyed observer successfully: $it")
            }
        }
        cse.destroyExecution(execution).also {
            LOG.debug("Destroyed execution successfully: $it")
        }
    }

    inner class CseSlave (
        fmuPath: File
    ) {
        private val slave: cse_slave
        val index: Int

        init {
            slave = cse.createLocalSlave(fmuPath.absolutePath)
            index = cse.addSlave(execution, slave)
        }

        fun getInteger(vr: Long): Int {
            val ref = IntArray(1)
            return getInteger(longArrayOf(vr), ref).let { ref[0] }
        }

        fun getInteger(vr: LongArray, ref: IntArray): Boolean {
            if (observer == null) {
                throw IllegalStateException("No observer has been set!")
            }
            return cse.getInteger(observer!!, index, vr, ref)
        }

        fun getReal(vr: Long): Double {
            val ref = DoubleArray(1)
            return getReal(longArrayOf(vr), ref).let { ref[0] }
        }

        fun getReal(vr: LongArray, ref: DoubleArray): Boolean {
            if (observer == null) {
                throw IllegalStateException("No observer has been set!")
            }
            return cse.getReal(observer!!, index, vr, ref)
        }

        fun setInteger(vr: Long, value: Int): Boolean {
            return setInteger(longArrayOf(vr), intArrayOf(value))
        }

        fun setInteger(vr: LongArray, values: IntArray): Boolean {
            return cse.setInteger(execution, index, vr, values)
        }

        fun setReal(vr: Long, value: Double): Boolean {
            return setReal(longArrayOf(vr), doubleArrayOf(value))
        }

        fun setReal(vr: LongArray, values: DoubleArray): Boolean {
            return cse.setReal(execution, index, vr, values)
        }

    }

    companion object {

        private val LOG: Logger = LoggerFactory.getLogger(CseExecution::class.java)

    }

}