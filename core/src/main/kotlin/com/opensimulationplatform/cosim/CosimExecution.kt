package com.opensimulationplatform.cosim

import com.opensimulationplatform.cosim.jni.CosimLibrary
import com.opensimulationplatform.cosim.jni.ExecutionPtr
import no.ntnu.ihb.fmi4j.export.fmi2.Fmi2Slave
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File

typealias SlaveRef = Int

class CosimExecution private constructor(
    private val executionPtr: ExecutionPtr
) : Closeable {

    private val slaves: MutableList<CosimSlave> = mutableListOf()
    private val observers: MutableList<CosimObserver> = mutableListOf()
    private val manipulators: MutableList<CosimManipulator> = mutableListOf()

    init {
        getSlaveInfos().forEach {
            slaves.add(CosimSlave(it.slaveRef, it.instanceName, executionPtr))
        }
    }

    val numSlaves: Int
        get() = CosimLibrary.getNumSlaves(executionPtr)

    val status: CosimExecutionStatus
        get() = CosimLibrary.getStatus(executionPtr)
            ?: throw IllegalStateException("Failed to retrieve status! Last reported error: ${CosimLibrary.getLastError()}")

    fun getSlaveByReference(ref: SlaveRef): CosimSlave {
        return getSlaveByReferenceOrNull(ref)
            ?: throw NoSuchElementException("No slave with index=$ref found!")
    }

    fun getSlaveByReferenceOrNull(ref: SlaveRef): CosimSlave? {
        return slaves.find { it.slaveRef == ref }
    }

    fun getSlaveByName(instanceName: String): CosimSlave {
        return getSlaveByNameOrNull(instanceName)
            ?: throw NoSuchElementException("No slave with name=$instanceName found!")
    }

    fun getSlaveByNameOrNull(instanceName: String): CosimSlave? {
        return slaves.find { it.instanceName == instanceName }
    }

    fun addSlave(fmuPath: File, instanceName: String): CosimSlave {
        require(fmuPath.extension == "fmu") { "File must have a .fmu ending!" }
        val slavePtr = CosimLibrary.createSlave(fmuPath.absolutePath, instanceName)
        if (slavePtr == 0L) {
            throw RuntimeException("Failed to create slave! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return addSlave(slavePtr)
    }

    private fun addSlave(slavePtr: Long): CosimSlave {
        return if (slavePtr != 0L) {
            val index = CosimLibrary.addSlave(executionPtr, slavePtr)
            val name = getSlaveInfos().find { it.slaveRef == index }!!.instanceName
            CosimLocalSlave(index, name, executionPtr, slavePtr).also {
                slaves.add(it)
            }
        } else {
            throw RuntimeException("Failed to add slave! Last reported error: ${CosimLibrary.getLastError()}")
        }
    }

    fun addJvmSlave(slave: Fmi2Slave): CosimSlave {
        val slavePtr = CosimLibrary.createJvmSlave(slave, slave.instanceName)
        if (slavePtr == 0L) {
            throw RuntimeException("Failed to create slave! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return addSlave(slavePtr)
    }

    fun start(): Boolean {
        return CosimLibrary.start(executionPtr)
    }

    @JvmOverloads
    fun step(numSteps: Long = 1): Boolean {
        return CosimLibrary.step(executionPtr, numSteps)
    }

    fun simulateUntil(targetTime: Double): Boolean {
        return CosimLibrary.simulateUntil(executionPtr, targetTime)
    }

    fun stop(): Boolean {
        return CosimLibrary.stop(executionPtr)
    }

    fun enableRealTimeSimulation(): Boolean {
        return CosimLibrary.enableRealTimeSimulation(executionPtr)
    }

    fun disableRealTimeSimulation(): Boolean {
        return CosimLibrary.disableRealTimeSimulation(executionPtr)
    }

    fun setRealTimeFactor(realTimeFactor: Double): Boolean {
        return CosimLibrary.setRealTimeFactorTarget(executionPtr, realTimeFactor)
    }

    fun connectIntegers(
        outputSlaveIndex: Int,
        outputValueRef: Long,
        inputSlaveIndex: Int,
        inputValueRef: Long
    ): Boolean {
        return CosimLibrary.connectIntegers(
            executionPtr,
            outputSlaveIndex,
            outputValueRef,
            inputSlaveIndex,
            inputValueRef
        )
    }

    fun connectReals(outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean {
        return CosimLibrary.connectReals(executionPtr, outputSlaveIndex, outputValueRef, inputSlaveIndex, inputValueRef)
    }

    fun getSlaveInfos(): List<CosimSlaveInfo> {
        return CosimLibrary.getSlaveInfos(executionPtr)
    }

    fun addLastValueObserver(): CosimLastValueObserver {
        val observer = CosimLibrary.createLastValueObserver()
        if (observer == 0L) {
            throw RuntimeException("Failed to create LastValueObserver! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return if (CosimLibrary.addObserver(executionPtr, observer)) {
            CosimLastValueObserver(observer).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed to add last value observer! Last reported error: ${CosimLibrary.getLastError()}")
        }
    }

    @JvmOverloads
    fun addFileObserver(logDir: File, cfgFile: File? = null): CosimFileObserver {
        if (!logDir.exists()) {
            logDir.mkdirs()
        }
        val observer = if (cfgFile == null) {
            CosimLibrary.createFileObserver(logDir.absolutePath)
        } else {
            CosimLibrary.createFileObserverFromCfg(logDir.absolutePath, cfgFile.absolutePath)
        }
        return if (CosimLibrary.addObserver(executionPtr, observer)) {
            CosimFileObserver(observer, logDir).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed to add file observer! Last reported error: ${CosimLibrary.getLastError()}")
        }
    }

    @JvmOverloads
    fun addTimeSeriesObserver(bufferSize: Int? = null): CosimTimeSeriesObserver {
        val observer = CosimLibrary.createTimeSeriesObserver(bufferSize)
        if (observer == 0L) {
            throw RuntimeException("Failed to create TimeSeriesObserver! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return if (CosimLibrary.addObserver(executionPtr, observer)) {
            CosimTimeSeriesObserver(observer).also {
                observers.add(it)
            }
        } else {
            throw RuntimeException("Failed to add time series observer! Last reported error: ${CosimLibrary.getLastError()}")
        }
    }

    fun addOverrideManipulator(): CosimOverrideManipulator {
        val manipulator = CosimLibrary.createOverrideManipulator()
        if (manipulator == 0L) {
            throw RuntimeException("Failed to create OverrideManipulator! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return if (CosimLibrary.addManipulator(executionPtr, manipulator)) {
            CosimOverrideManipulator(manipulator).also {
                manipulators.add(it)
            }
        } else {
            throw RuntimeException("Failed to add override manipulator! Last reported error: ${CosimLibrary.getLastError()}")
        }
    }

    fun loadScenario(scenarioFile: File): CosimScenario {
        val scenarioManager = CosimLibrary.createScenarioManager()
        if (scenarioManager == 0L) {
            throw RuntimeException("Failed to create ScenarioManager! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return if (CosimLibrary.addManipulator(executionPtr, scenarioManager)) {
            CosimLibrary.loadScenario(executionPtr, scenarioManager, scenarioFile.absolutePath).also { success ->
                if (success) {
                    LOG.debug("Loaded scenario '${scenarioFile.name}' successfully!")
                } else {
                    throw RuntimeException("Error loading scenario '${scenarioFile.name}'! Last reported error: ${CosimLibrary.getLastError()}")
                }

            }
            CosimScenario(scenarioManager).also {
                manipulators.add(it)
            }
        } else {
            throw RuntimeException("Failed add scenario manager! Last reported error: ${CosimLibrary.getLastError()}")
        }

    }

    fun addStepEventListener(listener: StepEventListener): CosimStepEventListener {
        val observer = CosimLibrary.createStepEventListener(listener)
        if (observer == 0L) {
            throw RuntimeException("Failed to create StepEventListener! Last reported error: ${CosimLibrary.getLastError()}")
        }
        return if (CosimLibrary.addObserver(executionPtr, observer)) {
            CosimStepEventListener(observer).also {
                observers.add(it)
                listener.addedToExecution(this)
            }
        } else {
            throw RuntimeException("Failed add scenario manager! Last reported error: ${CosimLibrary.getLastError()}")
        }
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
        CosimLibrary.destroyExecution(executionPtr).also { success ->
            if (success) {
                LOG.debug("Destroyed execution successfully!")
            } else {
                LOG.error("Error destroying execution!")
            }
        }
    }

    companion object {

        private val LOG: Logger = LoggerFactory.getLogger(CosimExecution::class.java)

        @JvmStatic
        fun create(stepSize: Double): CosimExecution {
            return create(0.0, stepSize)
        }

        @JvmStatic
        fun create(startTime: Double, stepSize: Double): CosimExecution {
            return CosimExecution(CosimLibrary.createExecution(startTime, stepSize))
        }

        @JvmStatic
        @JvmOverloads
        fun createFromSsp(sspPath: File, startTime: Double? = null, stepSize: Double? = null): CosimExecution {
            if (!sspPath.exists()) {
                throw NoSuchFileException(sspPath)
            }
            return CosimExecution(CosimLibrary.createSspExecution(sspPath.absolutePath, startTime, stepSize))
        }

        @JvmStatic
        @JvmOverloads
        fun createFromOspConfig(configPath: File, startTime: Double? = null): CosimExecution {
            if (!configPath.exists()) {
                throw NoSuchFileException(configPath)
            }
            return CosimExecution(CosimLibrary.createOspConfigExecution(configPath.absolutePath, startTime))
        }

    }

}

class CosimExecutionStatus(
    val currentTime: Double,
    val realTimeFactor: Double,
    val realTimeFactorTarget: Double,
    val realTimeSimulation: Boolean,
    private val stateId: Int,
    private val errorCodeId: Int
) {

    val state: CosimExecutionState by lazy {
        CosimExecutionState.valueOf(stateId)
    }

    val errorCode: CosimErrorCode by lazy {
        CosimErrorCode.valueOf(errorCodeId)
    }

    override fun toString(): String {
        return "CosimExecutionStatus(currentTime=$currentTime, state=$state, errorCode=$errorCode, realTimeFactor=$realTimeFactor, isRealTimeSimulation=$realTimeSimulation)"
    }

}


enum class CosimExecutionState(
    private val code: Int
) {
    UNKNOWN(-1),
    STOPPED(0),
    RUNNING(1),
    ERROR(2);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CosimExecutionState {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}
