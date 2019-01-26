package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File

class CseExecutionTest {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CseExecutionTest::class.java)

        val testFmu = TestFMUs.fmi20().cs()
            .vendor("20sim").version("4.6.4.8004")
            .name("ControlledTemperature").file()

    }

    @Test
    fun test1() {

        val stepSize = 1.0 / 100
        val numSteps = 10L

        CseExecution.create(stepSize).use { execution ->

            val observer = execution.addMembufferObserver()

            val slave = execution.addSlave(testFmu)

            execution.getSlaveInfos().apply {
                Assertions.assertEquals(0, this[0].index)
                Assertions.assertEquals("ControlledTemperature", this[0].name)
            }

            Assertions.assertTrue(execution.step(numSteps))

            execution.getStatus().apply {
                Assertions.assertEquals(stepSize * numSteps, this.currentTime, 1E-6)
            }

            Assertions.assertEquals(298.15, observer.getReal(slave, 46))

            observer.getRealSamples(slave, 46, 0, 5).also { samples ->
                Assertions.assertArrayEquals(doubleArrayOf(0.0, 298.15, 298.15, 298.15, 298.15), samples.values)
                Assertions.assertArrayEquals(doubleArrayOf(0.0, 0.01, 0.02, 0.030000000000000002, 0.04), samples.times)
                Assertions.assertArrayEquals(longArrayOf(0, 1, 2, 3, 4), samples.steps)
            }

            observer.getRealSamplesDirect(slave, 46, 0, 5).also { samples ->
                Assertions.assertArrayEquals(doubleArrayOf(0.0, 298.15, 298.15, 298.15, 298.15), samples.values)
                Assertions.assertArrayEquals(doubleArrayOf(0.0, 0.01, 0.02, 0.030000000000000002, 0.04), samples.times)
                Assertions.assertArrayEquals(longArrayOf(0, 1, 2, 3, 4), samples.steps)
            }

        }

    }

    @Test
    fun test2() {

        CseExecution.create(1.0 / 100).use { execution ->

            val logDir = File("build/results");
            execution.addFileObserver(logDir)

            val observer = execution.addMembufferObserver()

            val slave = execution.addSlave(testFmu)
            Assertions.assertTrue(execution.step(10))

            Assertions.assertEquals(298.15, observer.getReal(slave, 46))

            execution.getStatus().apply {
                Assertions.assertEquals(0.1, this.currentTime)
            }

            Assertions.assertTrue(logDir.listFiles().isNotEmpty())

        }

    }

}