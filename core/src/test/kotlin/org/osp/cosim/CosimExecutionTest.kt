package org.osp.cosim

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicLong

class CosimExecutionTest {

    private companion object {

        val LOG: Logger = LoggerFactory.getLogger(CosimExecutionTest::class.java)

        val testFmu = File(CosimExecutionTest::class.java.classLoader
                .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu")!!.file)

    }

    @Test
    fun test1() {

        val numSteps = 10L
        val stepSize = 1.0 / 100

        CosimExecution.create(stepSize).use { execution ->

            val slave = execution.addSlave(testFmu, "ControlledTemperature")
            val observer = execution.addTimeSeriesObserver()
            Assertions.assertTrue(observer.startObserving(slave.slaveRef, slave.getVariable("Temperature_Room")))

            execution.getSlaveInfos().apply {
                Assertions.assertEquals(0, this[0].slaveRef)
                Assertions.assertEquals("ControlledTemperature", this[0].instanceName)
            }

            Assertions.assertTrue(execution.step(numSteps))

            val nSamples = 6
            val samples = observer.getRealSamples(slave.slaveRef, 47 /*vr*/, 1, nSamples)
            Assertions.assertEquals(nSamples, samples.size)

            execution.status.apply {
                Assertions.assertEquals(stepSize * numSteps, this.currentTime, 1E-6)
            }

        }

    }

    @Test
    fun test2() {

        CosimExecution.create(1.0 / 100).use { execution ->

            val logDir = File("build/results")
            execution.addFileObserver(logDir)

            val slave = execution.addSlave(testFmu, "slave")
            slave.modelDescription.variables.also {

                Assertions.assertEquals(19, it[0].valueReference)
                Assertions.assertEquals("Ground.p1.u", it[0].name)

            }

            Assertions.assertTrue(execution.step(10))
            execution.status.apply {
                Assertions.assertEquals(0.1, this.currentTime)
            }

            Assertions.assertTrue(!logDir.listFiles().isNullOrEmpty())

            execution.addLastValueObserver().use { observer ->

                observer.getReal(slave.slaveRef, 1).also {
                    Assertions.assertEquals(298.0, it)
                }

            }

            Assertions.assertTrue(execution.step(10))
            execution.status.apply {
                Assertions.assertEquals(0.2, this.currentTime)
            }

        }

    }

    @Test
    fun testStepEvent() {

        CosimExecution.create(1.0 / 100).use { execution ->

            val counter = AtomicLong(0)
            val onSimulationInitializedCalled = AtomicBoolean(false)
            execution.addStepEventListener(object : StepEventAdapter() {

                override fun onSimulationInitialized(currentTime: Double) {
                    onSimulationInitializedCalled.set(true)
                }

                override fun onStepComplete(stepNumber: Long, currentTime: Double, lastStepSize: Double) {
                    counter.getAndIncrement()
                }

            })

            val numSteps = 10L
            execution.step(numSteps)
            Assertions.assertEquals(numSteps, counter.get())

            Assertions.assertTrue(onSimulationInitializedCalled.get())

        }
    }

    @Test
    fun testStepEvent2() {

        CosimExecution.create(1.0 / 100).use { execution ->

            execution.addSlave(testFmu, "slave")

            val counter = AtomicLong(0)
            execution.addStepEventListener(object : StepEventAdapter() {
                override fun onStepComplete(stepNumber: Long, currentTime: Double, lastStepSize: Double) {
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
