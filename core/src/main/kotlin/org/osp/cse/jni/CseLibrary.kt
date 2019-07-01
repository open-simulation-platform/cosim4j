package org.osp.cse.jni

import org.osp.cse.*
import org.osp.util.sharedLibExtension
import org.osp.util.libPrefix
import java.io.File
import java.io.FileOutputStream
import java.nio.ByteBuffer
import java.nio.file.Files

typealias cse_error_code = Int
typealias cse_execution = Long
typealias cse_slave = Long
typealias cse_observer = Long
typealias cse_manipulator = Long

object CseLibrary {

    init {

        val libName = "${libPrefix}csecorejni.$sharedLibExtension"
        val tempDir = Files.createTempDirectory("cse-core4j_").toFile()
        val lib = File(tempDir, libName)
        try {
            CseLibrary::class.java.classLoader.getResourceAsStream("native/$libName").use { `is` ->
                FileOutputStream(lib).use { fos ->
                    `is`.copyTo(fos)
                }
            }
            System.load(lib.absolutePath)
        } catch (ex: Exception) {
            tempDir.deleteRecursively()
            throw ex
        }
        lib.deleteOnExit()
        tempDir.deleteOnExit()

    }


    private external fun getLastErrorCode_(): cse_error_code

    /**
     *  Returns the error code associated with the last reported error.
     *
     *  Most functions in this library will indicate that an error occurred by
     *  returning -1 or `NULL`, after which this function can be called to
     *  obtain more detailed information about the problem.
     *
     *  This function must be called from the thread in which the error occurred,
     *  and before any new calls to functions in this library (with the exception
     *  of `cse_last_error_message()`).
     *
     *  @return The error code associated with the last reported error.
     */
    fun getLastErrorCode(): CseErrorCode {
        return CseErrorCode.valueOf(getLastErrorCode_())
    }

    /**
     *  Returns a textual description of the last reported error.
     *
     *  Most functions in this library will indicate that an error occurred by
     *  returning -1 or `NULL`, after which this function can be called to
     *  obtain more detailed information about the problem.
     *
     *  This function must be called create the thread in which the error occurred,
     *  and before any new calls to functions in this library (with the exception
     *  of `cse_last_error_code()`).
     *
     * @return A textual description of the last reported error.
     * The pointer is only guaranteed to remain valid until the next time a function
     * in this library is called (with the exception of `cse_last_error_code()`).
     */
    external fun getLastErrorMessage(): String

    /**
     * Creates a new execution.
     *
     * @param startTime  The (logical) time point at which the simulation should start.
     * @param stepSize The execution step size.
     *
     * @return A pointer to an object which holds the execution state, or NULL on error.
     */
    external fun createExecution(startTime: Double, stepSize: Double): cse_execution

    /**
     * Creates a new execution based on a SystemStructure.ssd file.
     *
     * @return A pointer to an object which holds the execution state, or NULL on error.
     */
    external fun createSspExecution(sspDir: String, startTime: Double): cse_execution

    /**
     * Destroys an execution.
     *
     * @return true on success and false on error.
     */
    external fun destroyExecution(execution: cse_execution): Boolean

    /**
     * Creates a new local slave.
     *
     * @param fmuPath Path to FMU.
     *
     * @return A pointer to an object which holds the local slave object, or NULL on error.
     */
    external fun createSlave(fmuPath: String): cse_slave

    /**
     *  Destroys a local slave.
     *
     *  @returns 0 on success and -1 on error.
     */
    external fun destroySlave(slave: cse_slave): Boolean

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
    external fun addSlave(execution: cse_execution, slave: cse_slave): Int

    /**
     * Advances an execution a number of time steps.
     *
     * @param execution The execution to be stepped.
     * @param numSteps The number of steps to advance the simulation execution.
     *
     * @return  0 on success and -1 on error.
     */
    external fun step(execution: cse_execution, numSteps: Long): Boolean


    /**
     * Starts an execution.
     * The execution will run until `cse_execution_stop()` is called.
     *
     * @param execution The execution to be started.
     *
     * @return 0 on success and -1 on error.
     */
    external fun start(execution: cse_execution): Boolean

    /**
     * Stops an execution.
     *
     * @param execution  The execution to be stopped.
     *
     * @return 0 on success and -1 on error.
     */
    external fun stop(execution: cse_execution): Boolean

    /**
     * Enables real time simulation for an execution.
     */
    external fun enableRealTimeSimulation(execution: cse_execution): Boolean

    /**
     * Disables real time simulation for an execution.
     */
    external fun disableRealTimeSimulation(execution: cse_execution): Boolean

    /**
     * Sets a custom real time factor.
     */
    external fun setRealTimeFactorTarget(execution: cse_execution, realTimeFactor: Double): Boolean

    /**
     * Returns execution status.
     *
     * @param execution The execution to get status create.
     * @param status A pointer to a cse_execution_status that will be filled with actual execution status.
     *
     * @return 0 on success and -1 on error.
     */
    external fun getStatus(execution: cse_execution, status: CseExecutionStatus): Boolean

    /**
     * Get the number of variables for a slave which has been added to an execution
     *
     * @return the number of variables for a slave which has been added to an execution, or -1 on error.
     */
    external fun getNumVariables(execution: cse_execution, slaveIndex: Int): Int

    /**
     *  Returns variable metadata for a slave.
     *
     *  @param execution The execution which the slave has been added to.
     *  @param slaveIndex The index of the slave.
     *  @param variables A pointer to an array of length `numVariables` which will be filled with actual `cse_variable_description` values.
     *
     *  @return The number of variables written to `variables` array or -1 on error.
     */
    external fun getVariables(execution: cse_execution, slaveIndex: Int, variables: Array<CseVariableDescription>): Boolean

    /**
     * Returns the number of slaves which have been added to an execution.
     *
     * @return the number of slaves which have been added to an execution.
     */
    external fun getNumSlaves(execution: cse_execution): Int

    /**
     * Returns slave infos.
     *
     * @param execution The execution to get slave infos from.
     * @param infos A pointer to an array of length `num_slaves` which will be filled with actual `slave_info` values.
     *
     * @return 0 on success and -1 on error.
     */
    external fun getSlaveInfos(execution: cse_execution, infos: Array<CseSlaveInfo>): Boolean

    /**
     * Retrieves the values of real variables for one slave.
     *
     * @return  0 on success and -1 on error.
     */
    external fun getReal(observer: cse_observer, slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean

//    /**
//     * Retrieves the values of real variables for one slave.
//     *
//     * @return  0 on success and -1 on error.
//     */
//    external fun getRealDirect(observer: cse_observer, slaveIndex: Int, vr: ByteBuffer, nvr: Int, ref: ByteBuffer): Boolean

    /**
     * Retrieves the values of integer variables for one slave.
     *
     * @return  0 on success and -1 on error.
     */
    external fun getInteger(observer: cse_observer, slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean

//    /**
//     * Retrieves the values of integer variables for one slave.
//     *
//     * @return  0 on success and -1 on error.
//     */
//    external fun getIntegerDirect(observer: cse_observer, slaveIndex: Int, vr: ByteBuffer, nvr: Int, ref: ByteBuffer): Boolean

//    /**
//     * Retrieves the values of integer variables for one slave.
//     *
//     * @return  0 on success and -1 on error.
//     */
//    external fun getBoolean(observer: cse_observer, slaveIndex: Int, vr: LongArray, ref: BooleanArray): Boolean

    /**
     * Sets the values of real variables for one slave.
     *
     * @return  0 on success and -1 on error.
     */
    external fun setReal(manipulator: cse_manipulator, slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean

//    /**
//     * Sets the values of real variables for one slave.
//     *
//     * @return  0 on success and -1 on error.
//     */
//    external fun setRealDirect(execution: cse_execution, slaveIndex: Int, vr: ByteBuffer, nvr: Int, values: ByteBuffer): Boolean


    /**
     *  Sets the values of integer variables for one slave.
     *
     *  @return  0 on success and -1 on error.
     */
    external fun setInteger(manipulator: cse_manipulator, slaveIndex: Int, vr: LongArray, values: IntArray): Boolean


    /**
     * Retrieves a series of observed values, step numbers and times for a real variable.
     *
     * @param observer the observer
     * @param slaveIndex  index of the slave
     * @param vr the variable index
     * @param stepNumber the step number to start from
     * @param nSamples the number of samples to read
     *
     */
    external fun getRealSamples(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int, samples: CseRealSamples): Boolean

//    /**
//     * Retrieves a series of observed values, step numbers and times for a real variable.
//     *
//     * @param observer the observer
//     * @param slaveIndex  index of the slave
//     * @param vr the variable index
//     * @param stepNumber the step number to start from
//     * @param nSamples the number of samples to read
//     *
//     */
//    external fun getRealSamplesDirect(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int, samples: CseRealSamplesDirect)


    /**
     * Retrieves a series of observed values, step numbers and times for a real variable.
     *
     * @param observer the observer
     * @param slaveIndex  index of the slave
     * @param vr the variable index
     * @param stepNumber the step number to start from
     * @param nSamples the number of samples to read
     *
     */
    external fun getIntegerSamples(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int, samples: CseIntegerSamples) : Boolean

//    /**
//     * Retrieves a series of observed values, step numbers and times for a real variable.
//     *
//     * @param observer the observer
//     * @param slaveIndex  index of the slave
//     * @param vr the variable index
//     * @param stepNumber the step number to start from
//     * @param nSamples the number of samples to read
//     *
//     */
//    external fun getIntegerSamplesDirect(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int, samples: CseIntegerSamplesDirect)

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
     * @return 0 on success and -1 on error.
     */
    external fun getStepNumbersForDuration(observer: cse_observer, slaveIndex: Int, duration: Double, steps: LongArray): Boolean

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
     * @return 0 on success and -1 on error.
     */
    external fun getStepNumbers(observer: cse_observer, slaveIndex: Int, begin: Double, end: Double, steps: LongArray): Boolean


    /**
     * Connects one real output variable to one real input variable.
     *
     * @param execution The execution.
     * @param outputSlaveIndex The source slave.
     * @param outputValueRef The source variable.
     * @param inputSlaveIndex The destination slave.
     * @param inputValueRef The destination variable.
     *
     * @return 0 on success and -1 on error.
     */
    external fun connectIntegers(execution: cse_execution, outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean

    /**
     * Connects one real output variable to one real input variable.
     *
     * @param execution The execution.
     * @param outputSlaveIndex The source slave.
     * @param outputValueRef The source variable.
     * @param inputSlaveIndex The destination slave.
     * @param inputValueRef The destination variable.
     *
     * @return 0 on success and -1 on error.
     */
    external fun connectReals(execution: cse_execution, outputSlaveIndex: Int, outputValueRef: Long, inputSlaveIndex: Int, inputValueRef: Long): Boolean

    /**
     * Creates an observer which logs variable values to file in csv format.
     *
     * @param logDir The directory where log files will be created.
     *
     * @return  The created observer.
     */
    external fun createFileObserver(logDir: String): cse_observer

    /**
     * Creates an observer which logs variable values to file in csv format. Variables to be logged
     * are specified in the supplied log config xml file.
     *
     * @param logDir The directory where log files will be created.
     * @param cfgDir The path to the provided config xml file.
     * @return The created observer.
     */
    external fun createFileObserverFromCfg(logDir: String, cfgDir: String): cse_observer

    /**
     * Creates an observer which buffers variable values in memory.
     *
     * To start observing a variable, `cse_observer_start_observing()` must be called.
     */
    external fun createTimeSeriesObserver(): cse_observer

    /**
     * Creates an observer which buffers up to `bufferSize` variable values in memory.
     *
     * To start observing a variable, `cse_observer_start_observing()` must be called.
     */
    external fun createTimeSeriesObserver(bufferSize: Int): cse_observer

    /**
     * Start observing a variable with a `time_series_observer`
     */
    external fun startObserving(observer: cse_observer): Boolean

    /**
     * Stop observing a variable with a `time_series_observer`
     */
    external fun stopObserving(observer: cse_observer): Boolean

    /**
     * Destroys an observer
     */
    external fun destroyObserver(observer: cse_observer): Boolean

    /**
     * Add an observer to an execution.
     *
     * @param execution The execution.
     * @param observer A pointer to an observer, which may not be null.
     * The observer may not previously have been added to any execution.
     *
     * @return 0 on success and -1 on error.
     */
    external fun addObserver(execution: cse_execution, observer: cse_observer): Boolean

    /**
     * Creates a manipulator for overriding variable values
     */
    external fun createManipulator(): cse_manipulator

    /**
     *  Add a manipulator to an execution.
     *
     *  @param execution The execution.
     *  @param manipulator  A pointer to a manipulator, which may not be null.
     *  The manipulator may not previously have been added to any execution.
     *
     *  @return 0 on success and -1 on error.
     */
    external fun addManipulator(execution: cse_execution, manipulator: cse_manipulator): Boolean

    /**
     * Destroys a manipulator
     */
    external fun destroyManipulator(manipulator: cse_manipulator): Boolean

    /**
     * Creates a manipulator for running scenarios.
     */
    external fun createScenarioManager(): cse_manipulator

    /**
     * Loads and executes a scenario from file.
     */
    external fun loadScenario(execution: cse_execution, manipulator: cse_manipulator, scenarioFile: String): Boolean

    /**
     * Checks if a scenario is running
     */
    external fun isScenarioRunning(manipulator: cse_manipulator): Boolean

    /**
     * Aborts the execution of a running scenario
     */
    external fun abortScenario(manipulator: cse_manipulator): Boolean

}