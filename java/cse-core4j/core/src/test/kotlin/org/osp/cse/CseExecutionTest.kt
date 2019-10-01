package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File
import java.util.concurrent.atomic.AtomicLong

class CseExecutionTest {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CseExecutionTest::class.java)

        val testFmu = File(CseExecutionTest::class.java.classLoader
                .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file)

    }

    @Test
    fun test1() {

        val numSteps = 10L
        val stepSize = 1.0 / 100

        CseExecution.create(stepSize).use { execution ->

            val slave = execution.addSlave(testFmu, "ControlledTemperature")!!
            val observer = execution.addTimeSeriesObserver()!!
            Assertions.assertTrue(observer.startObserving(slave.index, slave.getVariable("Temperature_Room")!!))

            execution.getSlaveInfos().apply {
                Assertions.assertEquals(0, this[0].index)
                Assertions.assertEquals("ControlledTemperature", this[0].name)
            }

            Assertions.assertTrue(execution.step(numSteps))

            val nSamples = 6
            val samples = observer.getRealSamples(slave.index, 47 /*vr*/, 1, nSamples)
            Assertions.assertEquals(nSamples, samples.size)

            execution.status!!.apply {
                Assertions.assertEquals(stepSize * numSteps, this.currentTime, 1E-6)
            }

        }

    }

    @Test
    fun test2() {

        CseExecution.create(1.0 / 100).use { execution ->

            val logDir = File("build/results")
            execution.addFileObserver(logDir)

            val slave = execution.addSlave(testFmu, "slave")!!
            slave.modelDescription.variables.also {

                Assertions.assertEquals(19, it[0].valueReference)
                Assertions.assertEquals("Ground.p1.u", it[0].name)

            }

            Assertions.assertTrue(execution.step(10))
            execution.status!!.apply {
                Assertions.assertEquals(0.1, this.currentTime)
            }

            Assertions.assertTrue(logDir.listFiles().isNotEmpty())

            execution.addLastValueObserver()!!.use { observer ->

                observer.getReal(slave.index, 1).also {
                    Assertions.assertEquals(298.0, it)
                }

            }

            Assertions.assertTrue(execution.step(10))
            execution.status!!.apply {
                Assertions.assertEquals(0.2, this.currentTime)
            }

        }

    }

    @Test
    fun testStepEvent() {

        CseExecution.create(1.0 / 100).use { execution ->

            val counter = AtomicLong(0)
            execution.addStepEventListener(object : StepEventListener {
                override fun post() {
                    counter.getAndIncrement()
                }
            })

            val numSteps = 10L
            execution.step(numSteps)
            Assertions.assertEquals(numSteps, counter.get())

        }
    }

    @Test
    fun testStepEvent2() {

        CseExecution.create(1.0 / 100).use { execution ->

            execution.addSlave(testFmu, "slave")

            val counter = AtomicLong(0)
            execution.addStepEventListener(object : StepEventListener {
                override fun post() {
                    counter.getAndIncrement()
                }
            })

            execution.start()
            Thread.sleep(100)
            execution.stop()

            Assertions.assertTrue(counter.get() > 10)

        }
    }

}
