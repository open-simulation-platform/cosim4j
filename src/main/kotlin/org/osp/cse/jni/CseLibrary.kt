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

//    external fun cse_last_error_message(): String

    /**
     * Creates a new execution.
     *
     * A pointer to an object which holds the execution state, or NULL on error.
     */
    external fun createExecution(startTime: Long, stepSize: Long): cse_execution

//    /**
//     * Creates a new execution based on a SystemStructure.ssd file.
//     *
//     * @return A pointer to an object which holds the execution state, or NULL on error.
//     */
//    external fun cse_ssp_execution_create(sspDir: String, startTime: Long): cse_execution

    /**
     * Destroys an execution.
     *
     * @return 0 on success and -1 on error.
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
    external fun addSlave(execution: cse_execution, slave: cse_slave): Boolean

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