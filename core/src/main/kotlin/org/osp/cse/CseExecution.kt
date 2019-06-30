package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.osp.cse.jni.cse_observer
import org.osp.cse.jni.cse_slave
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File


class CseExecution private constructor(
        private val execution: cse_execution
) : Closeable {

    private val status = CseExecutionStatus()
    private val observers: MutableList<CseObserver> = mutableListOf()
    private val manipulators: MutableList<CseManipulator> = mutableListOf()

    val numSlaves: Int
        get() = CseLibrary.getNumSlaves(execution)

    fun getStatus(): CseExecutionStatus {
        return status.also {
            CseLibrary.getStatus(execution, status)
        }
    }

    fun addSlave(fmuPath: File): CseSlave {
        return CseLibrary.createLocalSlave(fmuPath.absolutePath).let { slavePointer ->
            val index = CseLibrary.addSlave(execution, slavePointer)
            CseSlave(index, slavePointer)
        }
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

    fun setRealTimeFactor(realTimeFactor: Double): Boolean {
        return CseLibrary.setRealTimeFactorTarget(execution, realTimeFactor)
    }

    fun connectIntegers(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectIntegers(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun connectReals(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun getSlaveInfos(): Array<CseSlaveInfo> {
        return Array(numSlaves) { CseSlaveInfo() }.also {
            CseLibrary.getSlaveInfos(execution, it)
        }
    }

    fun addFileObserver(logDir: File, cfgDir: File? = null): CseFileObserver {
        if (!logDir.exists()) {
            logDir.mkdirs()
        }
        val observer = if (cfgDir == null) {
            CseLibrary.createFileObserver(logDir.absolutePath)
        } else {
            CseLibrary.createFileObserverFromCfg(logDir.absolutePath, cfgDir.absolutePath)
        }
        CseLibrary.addObserver(execution, observer)
        return CseFileObserver(observer, logDir).also {
            observers.add(it)
        }
    }

    fun addTimeSeriesObserver(bufferSize: Int? = null): CseTimeSeriesObserver {
        val observer = if (bufferSize == null) {
            CseLibrary.createTimeSeriesObserver()
        } else {
            CseLibrary.createTimeSeriesObserver(bufferSize)
        }
        CseLibrary.addObserver(execution, observer)
        return CseTimeSeriesObserver(observer).also {
            observers.add(it)
        }
    }

    fun loadScenario(scenarioFile: File): CseScenario {
        val manipulator = CseLibrary.createManipulator()
        CseLibrary.loadScenario(execution, manipulator, scenarioFile.absolutePath)
        return CseScenario(manipulator).also {
            manipulators.add(it)
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
        fun create(sspDir: File, startTime: Double): CseExecution {
            return CseLibrary.createSspExecution(sspDir.absolutePath, startTime).let {
                CseExecution((it))
            }
        }

    }

}
