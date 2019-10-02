package org.osp.cse

import no.ntnu.ihb.fmi4j.Fmi2Slave
import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ExecutionPtr
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File
import java.lang.RuntimeException


class CseExecution private constructor(
        private val executionPtr: ExecutionPtr
) : Closeable {

    private val slaves: MutableList<CseSlave> = mutableListOf()
    private val observers: MutableList<CseObserver> = mutableListOf()
    private val manipulators: MutableList<CseManipulator> = mutableListOf()

    init {
        getSlaveInfos().forEach {
            slaves.add(CseSlave(it.index, it.name, executionPtr))
        }
    }

    val numSlaves: Int
        get() = CseLibrary.getNumSlaves(executionPtr)

    val status: CseExecutionStatus?
        get() = CseLibrary.getStatus(executionPtr)

    fun getSlave(index: Int): CseSlave? {
        return slaves.find { it.index == index }
    }

    fun getSlave(name: String): CseSlave? {
        return slaves.find { it.name == name }
    }

    fun addSlave(fmuPath: File, instanceName: String): CseSlave? {
        return addSlave(CseLibrary.createSlave(fmuPath.absolutePath, instanceName))
    }

    fun addSlave(slave: Fmi2Slave, instanceName: String): CseSlave? {
        return addSlave(CseLibrary.createJvmSlave(slave, instanceName))
    }

    private fun addSlave(slavePtr: Long): CseSlave? {
        return if (slavePtr != 0L) {
            val index = CseLibrary.addSlave(executionPtr, slavePtr)
            val name = getSlaveInfos().find { it.index == index }!!.name
            CseLocalSlave(index, name, executionPtr, slavePtr).also {
                slaves.add(it)
            }
        } else {
            throw RuntimeException("Failed to add slave! Last reported error: ${CseLibrary.getLastError()}")
        }
    }

    fun start(): Boolean {
        return CseLibrary.start(executionPtr)
    }

    @JvmOverloads
    fun step(numSteps: Long = 1): Boolean {
        return CseLibrary.step(executionPtr, numSteps)
    }

    fun simulateUntil(targetTime: Double): Boolean {
        return CseLibrary.simulateUntil(executionPtr, targetTime)
    }

    fun stop(): Boolean {
        return CseLibrary.stop(executionPtr)
    }

    fun enableRealTimeSimulation(): Boolean {
        return CseLibrary.enableRealTimeSimulation(executionPtr)
    }

    fun disableRealTimeSimulation(): Boolean {
        return CseLibrary.disableRealTimeSimulation(executionPtr)
    }

    fun setRealTimeFactor(realTimeFactor: Double): Boolean {
        return CseLibrary.setRealTimeFactorTarget(executionPtr, realTimeFactor)
    }

    fun connectIntegers(outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean {
        return CseLibrary.connectIntegers(executionPtr, outputSlaveIndex, outputValueRef, inputSlaveIndex, inputValueRef)
    }

    fun connectReals(outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(executionPtr, outputSlaveIndex, outputValueRef, inputSlaveIndex, inputValueRef)
    }

    fun getSlaveInfos(): List<CseSlaveInfo> {
        return CseLibrary.getSlaveInfos(executionPtr)
    }

    fun addLastValueObserver(): CseLastValueObserver {
        val observer = CseLibrary.createLastValueObserver()
        if (observer == 0L) {
            throw RuntimeException("Failed to create LastValueObserver! Last reported error: ${CseLibrary.getLastError()}")
        }
        return if (CseLibrary.addObserver(executionPtr, observer)) {
            CseLastValueObserver(observer).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed to add last value observer! Last reported error: ${CseLibrary.getLastError()}")
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
        return if (CseLibrary.addObserver(executionPtr, observer)) {
            CseFileObserver(observer, logDir).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed to add file observer! Last reported error: ${CseLibrary.getLastError()}")
        }
    }

    @JvmOverloads
    fun addTimeSeriesObserver(bufferSize: Int? = null): CseTimeSeriesObserver {
        val observer = CseLibrary.createTimeSeriesObserver(bufferSize)
        if (observer == 0L) {
            throw RuntimeException("Failed to create TimeSeriesObserver! Last reported error: ${CseLibrary.getLastError()}")
        }
        return if (CseLibrary.addObserver(executionPtr, observer)) {
            CseTimeSeriesObserver(observer).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed to add time series observer! Last reported error: ${CseLibrary.getLastError()}")
        }
    }

    fun addOverrideManipulator(): CseOverrideManipulator {
        val manipulator = CseLibrary.createOverrideManipulator()
        if (manipulator == 0L) {
            throw RuntimeException("Failed to create OverrideManipulator! Last reported error: ${CseLibrary.getLastError()}")
        }
        return if (CseLibrary.addManipulator(executionPtr, manipulator)) {
            CseOverrideManipulator(manipulator).also {
                manipulators.add(it)
            }
        } else {
            throw RuntimeException("Failed to add override manipulator! Last reported error: ${CseLibrary.getLastError()}")
        }
    }

    fun loadScenario(scenarioFile: File): CseScenario {
        val scenarioManager = CseLibrary.createScenarioManager()
        if (scenarioManager == 0L) {
            throw RuntimeException("Failed to create ScenarioManager! Last reported error: ${CseLibrary.getLastError()}")
        }
        return if (CseLibrary.addManipulator(executionPtr, scenarioManager)) {
            CseLibrary.loadScenario(executionPtr, scenarioManager, scenarioFile.absolutePath).also { success ->
                if (success) {
                    LOG.debug("Loaded scenario '${scenarioFile.name}' successfully!")
                } else {
                    throw RuntimeException("Error loading scenario '${scenarioFile.name}'! Last reported error: ${CseLibrary.getLastError()}")
                }

            }
            CseScenario(scenarioManager).also {
                manipulators.add(it)
            }
        } else {
            throw RuntimeException("Failed add scenario manager! Last reported error: ${CseLibrary.getLastError()}")
        }

    }

    fun addStepEventListener(listener: StepEventListener): CseStepEventListener {
        val observer = CseLibrary.createStepEventListener(listener)
        if (observer == 0L) {
            throw RuntimeException("Failed to create StepEventListener! Last reported error: ${CseLibrary.getLastError()}")
        }
        return if (CseLibrary.addObserver(executionPtr, observer)) {
            CseStepEventListener(observer).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed add scenario manager! Last reported error: ${CseLibrary.getLastError()}")
        }

    }

    fun setInitialRealValue(slaveIndex: Int, vr: Long, value: Double) {
        CseLibrary.setInitialRealValue(executionPtr, slaveIndex, vr, value);
    }

    fun setInitialRealValue(slaveIndex: Int, name: String, value: Double) {
        getSlave(slaveIndex)?.getVariable(name)?.valueReference?.let {
            setInitialRealValue(slaveIndex, it, value)
        } ?: throw IllegalArgumentException("Could not find valueReference for variable named `$name` and slaveIndex=$slaveIndex")
    }

    fun setInitialIntegerValue(slaveIndex: Int, vr: Long, value: Int) {
        CseLibrary.setInitialIntegerValue(executionPtr, slaveIndex, vr, value);
    }

    fun setInitialIntegerValue(slaveIndex: Int, name: String, value: Int) {
        getSlave(slaveIndex)?.getVariable(name)?.valueReference?.let {
            setInitialIntegerValue(slaveIndex, it, value)
        } ?: throw IllegalArgumentException("Could not find valueReference for variable named `$name` and slaveIndex=$slaveIndex")
    }

    fun setInitialBooleanValue(slaveIndex: Int, vr: Long, value: Boolean) {
        CseLibrary.setInitialBooleanValue(executionPtr, slaveIndex, vr, value);
    }

    fun setInitialBooleanValue(slaveIndex: Int, name: String, value: Boolean) {
        getSlave(slaveIndex)?.getVariable(name)?.valueReference?.let {
            setInitialBooleanValue(slaveIndex, it, value)
        } ?: throw IllegalArgumentException("Could not find valueReference for variable named `$name` and slaveIndex=$slaveIndex")
    }

    fun setInitialStringValue(slaveIndex: Int, vr: Long, value: String) {
        CseLibrary.setInitialStringValue(executionPtr, slaveIndex, vr, value);
    }

    fun setInitialRealValue(slaveIndex: Int, name: String, value: String) {
        getSlave(slaveIndex)?.getVariable(name)?.valueReference?.let {
            setInitialStringValue(slaveIndex, it, value)
        } ?: throw IllegalArgumentException("Could not find valueReference for variable named `$name` and slaveIndex=$slaveIndex")
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
        CseLibrary.destroyExecution(executionPtr).also { success ->
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
        fun createFromSsp(sspDir: File, startTime: Double? = null, stepSize: Double? = null): CseExecution {
            if (!sspDir.exists()) {
                throw NoSuchFileException(sspDir)
            }
            sspDir.listFiles()?.find {
                it.name == sspFileName
            } ?: throw IllegalArgumentException("Directory $sspDir contains no $sspFileName")
            return CseExecution(CseLibrary.createSspExecution(sspDir.absolutePath, startTime, stepSize))
        }

    }

}

class CseExecutionStatus(
        val currentTime: Double,
        val realTimeFactor: Double,
        val realTimeFactorTarget: Double,
        val realTimeSimulation: Boolean,
        private val stateId: Int,
        private val errorCodeId: Int
) {

    val state: CseExecutionState by lazy {
        CseExecutionState.valueOf(stateId)
    }

    val errorCode: CseErrorCode by lazy {
        CseErrorCode.valueOf(errorCodeId)
    }

    override fun toString(): String {
        return "CseExecutionStatus(currentTime=$currentTime, state=$state, errorCode=$errorCode, realTimeFactor=$realTimeFactor, isRealTimeSimulation=$realTimeSimulation)"
    }

}


enum class CseExecutionState(
        private val code: Int
) {
    UNKNOWN(-1),
    STOPPED(0),
    RUNNING(1),
    ERROR(2);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseExecutionState {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw java.lang.IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}
