package org.osp.cse.jni

import org.osp.cse.*
import org.osp.util.libExtension
import org.osp.util.libPrefix

typealias cse_execution = Long
typealias cse_slave = Long
typealias cse_observer = Long

object CseLibrary {

    init {

        CseLibrary::class.java.classLoader.apply {

            getResource("native/cse/${libPrefix}csecorecpp.$libExtension").file.also {
                System.load(it)
            }
            getResource("native/cse/${libPrefix}csecorec.$libExtension").file.also {
                System.load(it)
            }
            getResource("native/cse/${libPrefix}csecore_jni.$libExtension").file.also {
                System.load(it)
            }

        }

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
    external fun createExecution(sspDir: String, startTime: Double): cse_execution

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
    external fun createLocalSlave(fmuPath: String): cse_slave

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
     * Returns execution status.
     *
     * @param execution The execution to get status create.
     * @param status A pointer to a cse_execution_status that will be filled with actual execution status.
     *
     * @return 0 on success and -1 on error.
     */
    external fun getStatus(execution: cse_execution, status: CseExecutionStatus): Boolean

    /**
     * Returns the number of slaves which have been added to an execution.
     *
     * @return  the number of slaves which have been added to an execution.
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
     * Sets the values of real variables for one slave.
     *
     * @return  0 on success and -1 on error.
     */
    external fun setReal(execution: cse_execution, slaveIndex: Int, vr: LongArray, values: DoubleArray): Boolean

    /**
     * Retrieves the values of real variables for one slave.
     *
     * @return  0 on success and -1 on error.
     */
    external fun getReal(observer: cse_observer, slaveIndex: Int, vr: LongArray, ref: DoubleArray): Boolean

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
    external fun getRealSamples(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int, samples: CseRealSamples)

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
    external fun getRealSamplesDirect(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int, samples: CseRealSamplesDirect)

    /**
     *  Sets the values of integer variables for one slave.
     *
     *  @return  0 on success and -1 on error.
     */
    external fun setInteger(execution: cse_execution, slaveIndex: Int, vr: LongArray, values: IntArray): Boolean

    /**
     * Retrieves the values of real variables for one slave.
     *
     * @return  0 on success and -1 on error.
     */
    external fun getInteger(observer: cse_observer, slaveIndex: Int, vr: LongArray, ref: IntArray): Boolean

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
    external fun getIntegerSamples(observer: cse_observer, slaveIndex: Int, vr: Long, stepNumber: Long, nSamples: Int): CseIntegerSamples

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
     * Creates an observer which buffers variable values in memory.
     *
     * @return  The created observer.
     */
    external fun createMembufferObserver(): cse_observer

    /**
     * Creates an observer which logs variable values to file in csv format.
     *
     * @param logDir The directory where log files will be created.
     *
     * @return  The created observer.
     */
    external fun createFileObserver(logDir: String): cse_observer

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
     * Destroys an observer
     */
    external fun destroyObserver(observer: cse_observer): Boolean

}