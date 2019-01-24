package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File

class CseExecutionTest {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CseExecutionTest::class.java)

        val test_fmu = File(CseExecutionTest::class.java.classLoader
            .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file)
    }

    @Test
    fun test1() {

        CseExecution(1.0/100).use { execution ->

            val observer = execution.addMemBufferObserver()

            val slave = execution.addSlave(test_fmu)
            Assertions.assertTrue(execution.step(10))

            LOG.info("${execution.getStatus()}")
            Assertions.assertEquals(298.15, observer.getReal(slave, 46))

        }

    }

    @Test
    fun test2() {

        CseExecution(1.0/100).use { execution ->

            val logDir = File("build/results");
            execution.addFileObserver(logDir)

            val observer = execution.addMemBufferObserver()

            val slave = execution.addSlave(test_fmu)
            Assertions.assertTrue(execution.step(10))

            Assertions.assertEquals(298.15, observer.getReal(slave, 46))

            LOG.info("${execution.getStatus()}")
            Assertions.assertTrue(logDir.listFiles().isNotEmpty())

        }

    }

}