package org.osp.cse.jni

import org.osp.currentOS
import org.osp.libExtension
import org.osp.libPrefix
import java.io.File
import java.io.FileOutputStream
import java.lang.Exception

typealias cse_execution = Long
typealias cse_slave = Long

class CseLibrary {

    /**
     *  Returns a textual description of the last reported error.
     *
     *  Most functions in this library will indicate that an error occurred by
     *  returning -1 or `NULL`, after which this function can be called to
     *  obtain more detailed information about the problem.
     *
     *  This function must be called from the thread in which the error occurred,
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
    external fun createExecution(sspDir: String, startTime: Long): cse_execution

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
    external fun enableRealTimeSimulation(execution: cse_execution): Int

    /**
     * Disables real time simulation for an execution.
     */
    external fun disableRealTimeSimulation(execution: cse_execution): Int


    /**
     * Returns the number of slaves which have been added to an execution.
     *
     * @return  the number of slaves which have been added to an execution.
     */
    external fun getNumSlaves(execution: cse_execution): Long



    private companion object {

        init {

            try {
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

            } catch (ex: Exception) {
                throw RuntimeException(ex)
            }

        }

    }

}