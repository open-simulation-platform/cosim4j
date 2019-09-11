package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.cse_execution
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File


class CseExecution private constructor(
        private val execution: cse_execution
) : Closeable {

    private val slaves: MutableList<CseSlave> = mutableListOf()
    private val observers: MutableList<CseObserver> = mutableListOf()
    private val manipulators: MutableList<CseManipulator> = mutableListOf()

    init {
        getSlaveInfos().forEach {
            slaves.add(CseSlave(it.index, it.name, 0L, execution))
        }
    }

    val numSlaves: Int
        get() = CseLibrary.getNumSlaves(execution)

    val status: CseExecutionStatus
        get() = CseExecutionStatus().also {
            CseLibrary.getStatus(execution, it)
        }

    fun getSlave(name: String): CseSlave? {
        return slaves.find { it.name == name }
    }

    fun addSlave(fmuPath: File): CseSlave {
        return CseLibrary.createSlave(fmuPath.absolutePath).let { slave ->
            val index = CseLibrary.addSlave(execution, slave)
            val name = getSlaveInfos().find { it.index == index }!!.name
            CseSlave(index, name, slave, execution).also {
                slaves.add(it)
            }
        }
    }

    fun start(): Boolean {
        return CseLibrary.start(execution)
    }

    @JvmOverloads
    fun step(numSteps: Long = 1): Boolean {
        return CseLibrary.step(execution, numSteps)
    }

    fun simulateUntil(targetTime: Double): Boolean {
        return CseLibrary.simulateUntil(execution, targetTime)
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

    fun connectIntegers(outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean {
        return CseLibrary.connectIntegers(execution, outputSlaveIndex, outputValueRef, inputSlaveIndex, inputValueRef)
    }

    fun connectIntegers(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectIntegers(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun connectReals(outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(execution, outputSlaveIndex, outputValueRef, inputSlaveIndex, inputValueRef)
    }

    fun connectReals(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(execution, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun getSlaveInfos(): Array<CseSlaveInfo> {
        return Array(numSlaves) { CseSlaveInfo() }.also {
            CseLibrary.getSlaveInfos(execution, it)
        }
    }

    fun addLastValueObserver(): CseLastValueObserver {
        val observer = CseLibrary.createLastValueObserver()
        CseLibrary.addObserver(execution, observer)
        return CseLastValueObserver(observer).also {
            observers.add(it)
        }
    }

    @JvmOverloads
    fun addFileObserver(logDir: File, cfgFile: File? = null): CseFileObserver {
        if (!logDir.exists()) {
            logDir.mkdirs()
        }
        val observer = if (cfgFile == null) {
            CseLibrary.createFileObserver(logDir.absolutePath)
        } else {
            CseLibrary.createFileObserverFromCfg(logDir.absolutePath, cfgFile.absolutePath)
        }
        CseLibrary.addObserver(execution, observer)
        return CseFileObserver(observer, logDir).also {
            observers.add(it)
        }
    }

    @JvmOverloads
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

    fun addOverrideManipulator(): CseOverrideManipulator {
        val manipulator = CseLibrary.createOverrideManipulator()
        CseLibrary.addManipulator(execution, manipulator)
        return CseOverrideManipulator(manipulator).also {
            manipulators.add(it)
        }
    }

    fun loadScenario(scenarioFile: File): CseScenario {
        val scenarioManager = CseLibrary.createScenarioManager()
        CseLibrary.addManipulator(execution, scenarioManager)
        CseLibrary.loadScenario(execution, scenarioManager, scenarioFile.absolutePath).also { success ->
            if (success) {
                LOG.debug("Loaded scenario '${scenarioFile.name}' successfully!")
            } else {
                LOG.error("Error loading scenario '${scenarioFile.name}'!")
            }

        }
        return CseScenario(scenarioManager).also {
            manipulators.add(it)
        }
    }

    fun addStepEventListener(listener: StepEventListener): CseStepEventListener {
        val observer = CseLibrary.createStepEventListener(listener)
        CseLibrary.addObserver(execution, observer)
        return CseStepEventListener(observer).also {
            observers.add(it)
        }
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: Double) {
        CseLibrary.setInitialRealValue(execution,slaveIndex, vr, value);
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: Int) {
        CseLibrary.setInitialIntegerValue(execution,slaveIndex, vr, value);
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: Boolean) {
        CseLibrary.setInitialBooleanValue(execution,slaveIndex, vr, value);
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: String) {
        CseLibrary.setInitialStringValue(execution,slaveIndex, vr, value);
    }

    override fun close() {
        observers.forEach {
            it.close()
        }
        manipulators.forEach {
            it.close()
        }
        slaves.forEach {
            it.close()
        }
        CseLibrary.destroyExecution(execution).also { success ->
            if (success) {
                LOG.debug("Destroyed execution successfully!")
            } else {
                LOG.error("Error destroying execution!")
            }
        }
    }

    companion object {

        private val LOG: Logger = LoggerFactory.getLogger(CseExecution::class.java)

        private const val sspFileName = "SystemStructure.ssd"

        @JvmStatic
        fun create(stepSize: Double): CseExecution {
            return create(0.0, stepSize)
        }

        @JvmStatic
        fun create(startTime: Double, stepSize: Double): CseExecution {
            return CseExecution(CseLibrary.createExecution(startTime, stepSize))
        }

        @JvmStatic
        @JvmOverloads
        fun createFromSsp(sspDir: File, startTime: Double = 0.0): CseExecution {
            if (!sspDir.exists()) {
                throw NoSuchFileException(sspDir)
            }
            sspDir.listFiles()?.find {
                it.name == sspFileName
            } ?: throw IllegalArgumentException("Directory $sspDir contains no $sspFileName")
            return CseExecution((CseLibrary.createSspExecution(sspDir.absolutePath, startTime)))
        }

    }

}

class CseExecutionStatus {

    var currentTime: Double = -1.0
        private set

    var realTimeFactor: Double = -1.0
        private set

    var realTimeSimulation: Boolean = false
        private set

    private var stateId: Int = -1

    val state: CseExecutionState by lazy {
        CseExecutionState.valueOf(stateId)
    }

    private var errorCodeId: Int = -1

    val errorCode: CseErrorCode by lazy {
        CseErrorCode.valueOf(errorCodeId)
    }

    override fun toString(): String {
        return "CseExecutionStatus(currentTime=$currentTime, state=$state, errorCode=$errorCode, realTimeFactor=$realTimeFactor, isRealTimeSimulation=$realTimeSimulation)"
    }

}
