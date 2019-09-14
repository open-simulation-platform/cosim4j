package org.osp.cse

import org.osp.cse.jni.CseLibrary
import org.osp.cse.jni.ExecutionPtr
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.io.File


class CseExecution private constructor(
        private val executionPtr: ExecutionPtr
) : Closeable {

    private val slaves: MutableList<CseSlave> = mutableListOf()
    private val observers: MutableList<CseObserver> = mutableListOf()
    private val manipulators: MutableList<CseManipulator> = mutableListOf()

    init {
        getSlaveInfos().forEach {
            slaves.add(CseSlave(it.index, it.name, 0L, executionPtr))
        }
    }

    val numSlaves: Int
        get() = CseLibrary.getNumSlaves(executionPtr)

    val status: CseExecutionStatus
        get() = CseExecutionStatus().also {
            CseLibrary.getStatus(executionPtr, it)
        }

    fun getSlave(name: String): CseSlave? {
        return slaves.find { it.name == name }
    }

    fun addSlave(fmuPath: File): CseSlave {
        return CseLibrary.createSlave(fmuPath.absolutePath).let { slave ->
            val index = CseLibrary.addSlave(executionPtr, slave)
            val name = getSlaveInfos().find { it.index == index }!!.name
            CseSlave(index, name, slave, executionPtr).also {
                slaves.add(it)
            }
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

    fun connectIntegers(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectIntegers(executionPtr, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun connectReals(outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(executionPtr, outputSlaveIndex, outputValueRef, inputSlaveIndex, inputValueRef)
    }

    fun connectReals(outputSlave: CseSlave, outputValueRef: Long, inputSlave: CseSlave, inputValueRef: Long): Boolean {
        return CseLibrary.connectReals(executionPtr, outputSlave.index, outputValueRef, inputSlave.index, inputValueRef)
    }

    fun getSlaveInfos(): List<CseSlaveInfo> {
        return  CseLibrary.getSlaveInfos(executionPtr)
    }

    fun addLastValueObserver(): CseLastValueObserver {
        val observer = CseLibrary.createLastValueObserver()
        CseLibrary.addObserver(executionPtr, observer)
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
        CseLibrary.addObserver(executionPtr, observer)
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
        CseLibrary.addObserver(executionPtr, observer)
        return CseTimeSeriesObserver(observer).also {
            observers.add(it)
        }
    }

    fun addOverrideManipulator(): CseOverrideManipulator {
        val manipulator = CseLibrary.createOverrideManipulator()
        CseLibrary.addManipulator(executionPtr, manipulator)
        return CseOverrideManipulator(manipulator).also {
            manipulators.add(it)
        }
    }

    fun loadScenario(scenarioFile: File): CseScenario {
        val scenarioManager = CseLibrary.createScenarioManager()
        CseLibrary.addManipulator(executionPtr, scenarioManager)
        CseLibrary.loadScenario(executionPtr, scenarioManager, scenarioFile.absolutePath).also { success ->
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
        CseLibrary.addObserver(executionPtr, observer)
        return CseStepEventListener(observer).also {
            observers.add(it)
        }
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: Double) {
        CseLibrary.setInitialRealValue(executionPtr,slaveIndex, vr, value);
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: Int) {
        CseLibrary.setInitialIntegerValue(executionPtr,slaveIndex, vr, value);
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: Boolean) {
        CseLibrary.setInitialBooleanValue(executionPtr,slaveIndex, vr, value);
    }

    fun setInitialValue(slaveIndex: Int, vr: Long, value: String) {
        CseLibrary.setInitialStringValue(executionPtr,slaveIndex, vr, value);
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
