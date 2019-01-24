package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_observer
import org.osp.cse.jni.cse_slave
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File


class CseExecution(
    startTime: Double,
    stepSize: Double
): Closeable {

    private val cse: CseLibrary
    private val execution: cse_execution
    private val observers = mutableListOf<CseObserver>()

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

    fun addMemBufferObserver(): CseMembufferObserver {
        return cse.createMembufferObserver().let {
            cse.addObserver(execution, it)
            CseMembufferObserver(it)
        }.also {
            observers.add(it)
        }
    }

    fun addFileObserver(logDir: File): CseFileObserver {
        if (!logDir.exists()) {
            logDir.mkdirs()
        }
        return cse.createFileObserver(logDir.absolutePath).let {
            cse.addObserver(execution, it)
            CseFileObserver(it)
        }.also {
            observers.add(it)
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



    override fun close() {
        observers.forEach {
            it.close()
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

    open inner class CseObserver protected constructor(
        private var observer_: cse_observer?
    ): Closeable {

        protected val observer: cse_observer = observer_ ?: throw IllegalStateException("${javaClass.simpleName} has been closed!")

        fun getStepNumbers(slave: CseSlave, begin: Double, end: Double): Pair<Long, Long> {
            val steps = LongArray(2)
            return cse.getStepNumbers(observer, slave.index, begin, end, steps).let {
                steps[0] to steps[1]
            }
        }

        override fun close() {
            observer_?.also {
                cse.destroyObserver(it).also { success ->
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

        fun getInteger(slave: CseSlave, vr: Long): Int {
            val ref = IntArray(1)
            return getInteger(slave, longArrayOf(vr), ref).let { ref[0] }
        }

        fun getInteger(slave: CseSlave, vr: LongArray, ref: IntArray): Boolean {
            return cse.getInteger(observer, slave.index, vr, ref)
        }

        fun getReal(slave: CseSlave, vr: Long): Double {
            val ref = DoubleArray(1)
            return getReal(slave, longArrayOf(vr), ref).let { ref[0] }
        }

        fun getReal(slave: CseSlave, vr: LongArray, ref: DoubleArray): Boolean {
            return cse.getReal(observer, slave.index, vr, ref)
        }

    }

    companion object {

        private val LOG: Logger = LoggerFactory.getLogger(CseExecution::class.java)

    }

}

