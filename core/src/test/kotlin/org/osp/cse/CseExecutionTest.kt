package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File

class CseExecutionTest {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CseExecutionTest::class.java)

        val testFmu = File(CseExecutionTest::class.java.classLoader
                .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file)

    }

    @Test
    fun test1() {

        val stepSize = 1.0 / 100
        val numSteps = 10L

        CseExecution.create(stepSize).use { execution ->

            execution.addSlave(testFmu)

            execution.getSlaveInfos().apply {
                Assertions.assertEquals(0, this[0].index)
                Assertions.assertEquals("ControlledTemperature", this[0].name)
            }

            Assertions.assertTrue(execution.step(numSteps))

            execution.getStatus().apply {
                Assertions.assertEquals(stepSize * numSteps, this.currentTime, 1E-6)
            }

        }

    }

    @Test
    fun test2() {

        CseExecution.create(1.0 / 100).use { execution ->

            val logDir = File("build/results");
            execution.addFileObserver(logDir)

            val slave = execution.addSlave(testFmu)
            slave.variables.forEach {
                println(it)
            }

            Assertions.assertTrue(execution.step(10))

            execution.getStatus().apply {
                Assertions.assertEquals(0.1, this.currentTime)
            }

            Assertions.assertTrue(logDir.listFiles().isNotEmpty())

        }

    }

}