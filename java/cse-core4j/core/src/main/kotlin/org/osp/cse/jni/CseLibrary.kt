package org.osp.cse.jni

import no.ntnu.ihb.fmi4j.Fmi2Slave
import org.osp.cse.*
import org.osp.util.isLinux
import org.osp.util.libPrefix
import org.osp.util.sharedLibExtension
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File
import java.io.FileOutputStream
import java.nio.file.Files

internal typealias NativeErrorCode = Int
internal typealias ExecutionPtr = Long
internal typealias SlavePtr = Long
internal typealias ObserverPtr = Long
internal typealias ManipulatorPtr = Long

object CseLibrary {

    private val LOG: Logger = LoggerFactory.getLogger(CseLibrary::class.java)

    external fun getLastError(): CseError

    /**
     * Creates a new execution.
     *
     * @param startTime  The (logical) time point at which the simulation should start.
     * @param stepSize The execution step size.
     *
     * @return A pointer to an object which holds the execution state, or NULL on error.
     */
    external fun createExecution(startTime: Double, stepSize: Double): ExecutionPtr

    /**
     * Creates a new execution based on a SystemStructure.ssd file.
     *
     * @return A pointer to an object which holds the execution state, or NULL on error.
     */
    private external fun createSspExecution(sspDir: String, startTimeDefined: Boolean, startTime: Double): ExecutionPtr

    /**
     * Creates a new execution based on a SystemStructure.ssd file.
     *
     * @return A pointer to an object which holds the execution state, or NULL on error.
     */
    private external fun createFixedStepSspExecution(sspDir: String, stepSize: Double, startTimeDefined: Boolean, startTime: Double): ExecutionPtr

    @JvmOverloads
    fun createSspExecution(sspDir: String, startTime: Double? = null, stepSize: Double? = null): ExecutionPtr {
        val startTimeDefined = startTime?.let { it >= 0 } ?: false
        return if (stepSize == null) {
            createSspExecution(sspDir, startTimeDefined, startTime ?: 0.0)
        } else {
            createFixedStepSspExecution(sspDir, stepSize, startTimeDefined, startTime ?: 0.0)
        }
    }

    /**
     * Destroys an execution.
     *
     * @return true on success and false on error.
     */
    external fun destroyExecution(execution: ExecutionPtr): Boolean

    /**
     * Creates a new local slave.
     *
     * @param fmuPath Path to FMU.
     * @param instanceName name of the instance
     *
     * @return A pointer to an object which holds the local slave object, or NULL on error.
     */
    external fun createSlave(fmuPath: String, instanceName: String): SlavePtr

    /**
     * Creates a new local slave.
     *
     * @param slave the JVM owned slave
     * @param instanceName name of the instance
     *
     * @return A pointer to an object which holds the local slave object, or NULL on error.
     */
    external fun createJvmSlave(slave: Fmi2Slave, instanceName: String): SlavePtr

    /**
     *  Destroys a local slave.
     *
     *  @returns true on success and false on error.
     */
    external fun destroySlave(slave: SlavePtr): Boolean

    /**
     *  Loads a co-simulation FMU, instantiates a slave based on it, and adds it
     *  to an execution.
     *
     *  The slave is owned by the execution and is destroyed along with it.
     *
     * @param execution The execution to which the slave should be added.
     * @param slave A pointer to a slave, which may not be null. The slave may not previously have been added to any execution.
     *
     * @return The slave's unique index in the execution, or -1 on error.
     */
    external fun addSlave(execution: ExecutionPtr, slave: SlavePtr): Int

    /**
     * Advances an execution a number of time steps.
     *
     * @param execution The execution to be stepped.
     * @param numSteps The number of steps to advance the simulation execution.
     *
     * @return  true on success and false on error.
     */
    external fun step(execution: ExecutionPtr, numSteps: Long): Boolean

    /**
     *  Advances an execution to a specific point in time.
     *
     *  @param execution The execution to be stepped.
     *  @param targetTime The point in time, which to advance the simulation execution.
     *
     *  @return true on success and false on error.
     */
    external fun simulateUntil(execution: ExecutionPtr, targetTime: Double): Boolean

    /**
     * Starts an execution.
     * The execution will run until `cse_execution_stop()` is called.
     *
     * @param execution The execution to be started.
     *
     * @return true on success and false on error.
     */
    external fun start(execution: ExecutionPtr): Boolean

    /**
     * Stops an execution.
     *
     * @param execution  The execution to be stopped.
     *
     * @return true on success and false on error.
     */
    external fun stop(execution: ExecutionPtr): Boolean

    /**
     * Enables real time simulation for an execution.
     */
    external fun enableRealTimeSimulation(execution: ExecutionPtr): Boolean

    /**
     * Disables real time simulation for an execution.
     */
    external fun disableRealTimeSimulation(execution: ExecutionPtr): Boolean

    /**
     * Sets a custom real time factor.
     */
    external fun setRealTimeFactorTarget(execution: ExecutionPtr, realTimeFactor: Double): Boolean

    /**
     * Returns execution status.
     *
     * @param execution The execution to get status create.
     *
     * @return the execution status.
     */
    external fun getStatus(execution: ExecutionPtr): CseExecutionStatus?

    /**
     * Returns the number of slaves which have been added to an execution.
     *
     * @return the number of slaves which have been added to an execution.
     */
    external fun getNumSlaves(execution: ExecutionPtr): Int

    /**
     * Returns slave infos.
     *
     * @param execution The execution to get slave infos from.
     * @param infos A pointer to an array of length `num_slaves` which will be filled with actual `slave_info` values.
     *
     * @return true on success and false on error.
     */
    external fun getSlaveInfos(execution: ExecutionPtr): List<CseSlaveInfo>

    /**
     * Retrieves the values of real variables for one slave.
     *
     * @return  true on success and false on error.
     */
    external fun getReal(observer: ObserverPtr, slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean

    /**
     * Retrieves the values of integer variables for one slave.
     *
     * @return  true on success and false on error.
     */
    external fun getInteger(observer: ObserverPtr, slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean

    /**
     * Retrieves the values of boolean variables for one slave.
     *
     * @return  true on success and false on error.
     */
    external fun getBoolean(observer: ObserverPtr, slaveIndex: Int, vr: LongArray, ref: BooleanArray): Boolean

    /**
     * Retrieves the values of boolean variables for one slave.
     *
     * @return  true on success and false on error.
     */
    external fun getString(observer: ObserverPtr, slaveIndex: Int, vr: LongArray, ref: Array<String>): Boolean

    /**
     * Sets the values of real variables for one slave.
     *
     * @return  true on success and false on error.
     */
    external fun setReal(manipulator: ManipulatorPtr, slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean

    external fun setInitialRealValue(execution: ExecutionPtr, slaveIndex: Int, vr: Long, value: Double): Boolean

    /**
     *  Sets the values of integer variables for one slave.
     *
     *  @return  true on success and false on error.
     */
    external fun setInteger(manipulator: ManipulatorPtr, slaveIndex: Int, vr: LongArray, values: IntArray): Boolean

    external fun setInitialIntegerValue(execution: ExecutionPtr, slaveIndex: Int, vr: Long, value: Int): Boolean

    /**
     *  Sets the values of boolean variables for one slave.
     *
     *  @return  true on success and false on error.
     */
    external fun setBoolean(manipulator: ManipulatorPtr, slaveIndex: Int, vr: LongArray, values: BooleanArray): Boolean

    external fun setInitialBooleanValue(execution: ExecutionPtr, slaveIndex: Int, vr: Long, value: Boolean): Boolean

    /**
     *  Sets the values of string variables for one slave.
     *
     *  @return  true on success and false on error.
     */
    external fun setString(manipulator: ManipulatorPtr, slaveIndex: Int, vr: LongArray, values: Array<String>): Boolean

    external fun setInitialStringValue(execution: ExecutionPtr, slaveIndex: Int, vr: Long, value: String): Boolean

    /**
     * Retrieves a series of observed values, step numbers and times for a real variable.
     *
     * @param observer the observer
     * @param slaveIndex  index of the slave
     * @param vr the value reference
     * @param stepNumber the step number to start from
     * @param nSamples the number of samples to read
     *
     */
    external fun getRealSamples(observer: ObserverPtr, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseRealSamples

    /**
     * Retrieves a series of observed values, step numbers and times for a real variable.
     *
     * @param observer the observer
     * @param slaveIndex  index of the slave
     * @param vr the value reference
     * @param stepNumber the step number to start from
     * @param nSamples the number of samples to read
     *
     */
    external fun getIntegerSamples(observer: ObserverPtr, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples

    /**
     * Retrieves the step numbers for a range given by a duration.
     *
     * Helper function which can be used in conjunction with `cse_observer_slave_get_xxx_samples()`
     * when it is desired to retrieve the latest available samples given a certain duration.
     *
     * Note: It is assumed that `steps` has a length of 2.
     *
     * @param observer the observer
     * @param slaveIndex index of the simulator
     * @param duration duration the duration to get step numbers for
     * @param steps the corresponding step numbers
     *
     * @return true on success and false on error.
     */
    external fun getStepNumbersForDuration(observer: ObserverPtr, slaveIndex: Int, duration: Double, steps: LongArray): Boolean

    /**
     * Retrieves the step numbers for a range given by two points in time.
     *
     * Helper function which can be used in conjunction with `cse_observer_slave_get_xxx_samples()`
     * when it is desired to retrieve samples between two points in time.
     *
     * Note: It is assumed that `steps` has a length of 2.
     *
     * @param observer the observer
     * @param slaveIndex index of the simulator
     * @param begin the start of the range
     * @param end the end of the range
     * @param steps the corresponding step numbers
     *
     * @return true on success and false on error.
     */
    external fun getStepNumbers(observer: ObserverPtr, slaveIndex: Int, begin: Double, end: Double, steps: LongArray): Boolean


    /**
     * Connects one real output variable to one real input variable.
     *
     * @param execution The execution.
     * @param outputSlaveIndex The source slave.
     * @param outputValueRef The source variable.
     * @param inputSlaveIndex The destination slave.
     * @param inputValueRef The destination variable.
     *
     * @return true on success and false on error.
     */
    external fun connectIntegers(execution: ExecutionPtr, outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean

    /**
     * Connects one real output variable to one real input variable.
     *
     * @param execution The execution.
     * @param outputSlaveIndex The source slave.
     * @param outputValueRef The source variable.
     * @param inputSlaveIndex The destination slave.
     * @param inputValueRef The destination variable.
     *
     * @return true on success and false on error.
     */
    external fun connectReals(execution: ExecutionPtr, outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean

    /**
     * Creates an observer which stores the last observed value for all variables.
     */
    external fun createLastValueObserver(): ObserverPtr

    /**
     * Creates an observer which logs variable values to file in csv format.
     *
     * @param logDir The directory where log files will be created.
     *
     * @return The created observer.
     */
    external fun createFileObserver(logDir: String): ObserverPtr

    /**
     * Creates an observer which logs variable values to file in csv format. Variables to be logged
     * are specified in the supplied log config xml file.
     *
     * @param logDir The directory where log files will be created.
     * @param cfgFile The path to the provided config xml file.
     * @return The created observer.
     */
    external fun createFileObserverFromCfg(logDir: String, cfgFile: String): ObserverPtr

    /**
     * Creates an observer which buffers up to `bufferSize` variable values in memory.
     *
     * To start observing a variable, `cse_observer_start_observing()` must be called.
     */
    external fun createTimeSeriesObserver(bufferSize: Int?): ObserverPtr

    /**
     * Start observing a variable with a `time_series_observer`
     */
    private external fun startObserving(observer: ObserverPtr, slaveIndex: Int, type: Int, vr: Long): Boolean

    fun startObserving(observer: ObserverPtr, slaveIndex: Int, type: CseVariableType, vr: Long): Boolean {
        return startObserving(observer, slaveIndex, type.code, vr)
    }

    /**
     * Stop observing a variable with a `time_series_observer`
     */
    external fun stopObserving(observer: ObserverPtr, slaveIndex: Int, type: CseVariableType, vr: Long): Boolean

    /**
     * Destroys an observer
     */
    external fun destroyObserver(observer: ObserverPtr): Boolean

    /**
     * Add an observer to an execution.
     *
     * @param execution The execution.
     * @param observer A pointer to an observer, which may not be null.
     * The observer may not previously have been added to any execution.
     *
     * @return true on success and false on error.
     */
    external fun addObserver(execution: ExecutionPtr, observer: ObserverPtr): Boolean

    /**
     * Creates a manipulator for overriding variable values
     */
    external fun createOverrideManipulator(): ManipulatorPtr

    /**
     *  Add a manipulator to an execution.
     *
     *  @param execution The execution.
     *  @param manipulator  A pointer to a manipulator, which may not be null.
     *  The manipulator may not previously have been added to any execution.
     *
     *  @return true on success and false on error.
     */
    external fun addManipulator(execution: ExecutionPtr, manipulator: ManipulatorPtr): Boolean

    /**
     * Destroys a manipulator
     */
    external fun destroyManipulator(manipulator: ManipulatorPtr): Boolean

    /**
     * Creates a manipulator for running scenarios.
     */
    external fun createScenarioManager(): ManipulatorPtr

    /**
     * Loads and executes a scenario from file.
     */
    external fun loadScenario(execution: ExecutionPtr, manipulator: ManipulatorPtr, scenarioFile: String): Boolean

    /**
     * Checks if a scenario is running
     */
    external fun isScenarioRunning(manipulator: ManipulatorPtr): Boolean

    /**
     * Aborts the execution of a running scenario
     */
    external fun abortScenario(manipulator: ManipulatorPtr): Boolean

    external fun createStepEventListener(listener: StepEventListener): ObserverPtr

    external fun getModelDescription(execution: ExecutionPtr, slaveIndex: Int): CseModelDescription

    /**
     *  Configures simple console logging.
     *
     *  Note that the library may produce log messages before this function is
     *  called, but then it uses the default or existing settings of the underlying
     *  logging framework (Boost.Log).
     *
     *  @return true on success and false on error.
     */
    private external fun setupSimpleConsoleLogging(): Boolean

    private external fun setLogLevel(level: Int): Boolean

    /**
     *  Installs a global severity level filter for log messages.
     *
     *  This function sets up a log message filter which ensures that only messages
     *  whose severity level is at least `level` will be printed.
     *
     *  @param level The minimum visible severity level.
     */
    fun setLogLevel(level: CseLogLevel) {
        setLogLevel(level.code)
    }


    init {

        val platform = if (isLinux) "linux" else "win"
        val tempDir = Files.createTempDirectory("cse_core4j_").toFile().also {
            it.deleteOnExit()
        }

        fun loadLib(libName: String) {
            val lib by lazy {
                File(tempDir, libName).also {
                    it.deleteOnExit()
                }
            }
            val relativeLibPath = "native/$platform/$libName"
            LOG.debug("Loading library '$libName'")
            CseLibrary::class.java.classLoader.getResourceAsStream(relativeLibPath)?.use {
                FileOutputStream(lib).use { fos ->
                    it.copyTo(fos)
                }
            } ?: throw IllegalStateException("No such resource $relativeLibPath")

            System.load(lib.absolutePath)

        }

        fun loadCse() {
            listOf(
                    "${libPrefix}csecorecpp.$sharedLibExtension",
                    "${libPrefix}csecorec.$sharedLibExtension",
                    "${libPrefix}csecorejni.$sharedLibExtension"
            ).forEach { loadLib(it) }
        }

        fun loadBoost() {

            val postfix = if (isLinux) {
                ".so.1.66.0"
            } else {
                "-vc141-mt-x64-1_66.dll"
            }

            listOf(
                    "${libPrefix}boost_context$postfix",
                    "${libPrefix}boost_date_time$postfix",
                    "${libPrefix}boost_system$postfix",
                    "${libPrefix}boost_filesystem$postfix",
                    "${libPrefix}boost_fiber$postfix",
                    "${libPrefix}boost_chrono$postfix",
                    "${libPrefix}boost_thread$postfix",
                    "${libPrefix}boost_log$postfix"
            ).forEach { loadLib(it) }
        }

        try {

            loadBoost()
            loadCse()

            setupSimpleConsoleLogging()
            setLogLevel(CseLogLevel.CSE_LOG_SEVERITY_INFO)

        } catch (ex: Exception) {
            tempDir.deleteRecursively()
            throw ex
        }

    }

}
