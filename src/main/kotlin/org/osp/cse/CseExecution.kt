package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_observer
import org.osp.cse.jni.cse_slave
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File


class CseExecution private constructor (
    private val execution: cse_execution
): Closeable {

    private val status = CseExecutionStatusImpl()
    private val observers: MutableList<CseObserver> = mutableListOf()

    val numSlaves: Int
        get() = CseLibrary.getNumSlaves(execution)


    fun getStatus(): CseExecutionStatus {
        return status.also {
            CseLibrary.getStatus(execution, status)
        }
    }

    fun addMemBufferObserver(): CseMembufferObserver {
        return CseLibrary.createMembufferObserver().let {
            CseLibrary.addObserver(execution, it)
            CseMembufferObserver(it)
        }.also {
            observers.add(it)
        }
    }

    fun addFileObserver(logDir: File): CseFileObserver {
        if (!logDir.exists()) {
            logDir.mkdirs()
        }
        return CseLibrary.createFileObserver(logDir.absolutePath).let {
            CseLibrary.addObserver(execution, it)
            CseFileObserver(it)
        }.also {
            observers.add(it)
        }
    }

    fun addSlave(fmuPath: File): CseSlave {
        return CseSlave(fmuPath)
    }

    fun start(): Boolean {
        return CseLibrary.start(execution)
    }

    fun step(numSteps: Long): Boolean {
        return CseLibrary.step(execution, numSteps)
    }

    fun stop(): Boolean {
        return CseLibrary.stop(execution)
    }

    fun enableRealTimeSimulation(): Boolean {
        return CseLibrary.enableRealTimeSimulation(execution)
    }

    fun disableRealTimeSimulation(): Boolean {
        return CseLibrary.disableRealTimeSimulation(execution)
    }

    fun connectIntegers(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectIntegers(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun connectReals(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun getSlaveInfos(): Array<CseSlaveInfo> {
        return Array(numSlaves) {CseSlaveInfo()}.also {
            CseLibrary.getSlaveInfos(execution, it)
        }
    }

    override fun close() {
        observers.forEach {
            it.close()
        }
        CseLibrary.destroyExecution(execution).also {
            LOG.debug("Destroyed execution successfully: $it")
        }
    }

    inner class CseSlave (
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

        fun setReal(vr: Long, value: Double): Boolean {
            return setReal(longArrayOf(vr), doubleArrayOf(value))
        }

        fun setReal(vr: LongArray, values: DoubleArray): Boolean {
            return CseLibrary.setReal(execution, index, vr, values)
        }

    }

    open inner class CseObserver protected constructor(
        private var observer_: cse_observer?
    ): Closeable {

        protected val observer: cse_observer = observer_ ?: throw IllegalStateException("${javaClass.simpleName} has been closed!")

        fun getStepNumbersForDuration(slave: CseSlave, duration: Double): Pair<Long, Long> {
            val steps = LongArray(2)
            return CseLibrary.getStepNumbersForDuration(observer, slave.index, duration, steps).let {
                steps[0] to steps[1]
            }
        }

        fun getStepNumbers(slave: CseSlave, begin: Double, end: Double): Pair<Long, Long> {
            val steps = LongArray(2)
            return CseLibrary.getStepNumbers(observer, slave.index, begin, end, steps).let {
                steps[0] to steps[1]
            }
        }

        override fun close() {
            observer_?.also {
                CseLibrary.destroyObserver(it).also { success ->
                    observer_ = null
                    LOG.debug("Destroyed instance of ${javaClass.simpleName} successfully: $success")
                }
            }
        }
    }

    inner class CseFileObserver(
        observer: cse_observer
    ): CseObserver(observer)

    inner class CseMembufferObserver(
        observer: cse_observer
    ): CseObserver(observer) {

        fun getReal(slave: CseSlave, vr: Long): Double {
            val ref = DoubleArray(1)
            return getReal(slave, longArrayOf(vr), ref).let { ref[0] }
        }

        fun getReal(slave: CseSlave, vr: LongArray, ref: DoubleArray): Boolean {
            return CseLibrary.getReal(observer, slave.index, vr, ref)
        }

        fun getRealSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples {
            return CseLibrary.getRealSamples(observer, slave.index, vr, stepNumber, nSamples)
        }

        fun getInteger(slave: CseSlave, vr: Long): Int {
            val ref = IntArray(1)
            return getInteger(slave, longArrayOf(vr), ref).let { ref[0] }
        }

        fun getInteger(slave: CseSlave, vr: LongArray, ref: IntArray): Boolean {
            return CseLibrary.getInteger(observer, slave.index, vr, ref)
        }

        fun getIntegerSamples(slave: CseSlave, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples {
            return CseLibrary.getIntegerSamples(observer, slave.index, vr, stepNumber, nSamples)
        }

    }

    companion object {

        private val LOG: Logger = LoggerFactory.getLogger(CseExecution::class.java)

        @JvmStatic
        fun create(stepSize: Double): CseExecution {
            return CseLibrary.createExecution(0.0, stepSize).let {
                CseExecution(it)
            }
        }

        @JvmStatic
        fun create(startTime: Double, stepSize: Double): CseExecution {
            return CseLibrary.createExecution(startTime, stepSize).let {
                CseExecution(it)
            }
        }

        @JvmStatic
        fun create(sspDir: File, stepSize: Double): CseExecution {
            return CseLibrary.createExecution(sspDir.absolutePath, stepSize).let {
                CseExecution((it))
            }
        }

    }

}

