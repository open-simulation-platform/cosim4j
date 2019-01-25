package org.osp.cse

import no.ntnu.ihb.fmi4j.importer.Fmu
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import kotlin.system.measureTimeMillis

@Disabled
class PerformanceTest {

    companion object {
        private val testFmu
            get() = TestFMUs.fmi20().cs()
                .vendor("20sim").version("4.6.4.8004")
                .name("ControlledTemperature").file()

        private val stop: Double = 100.0
        private val stepSize: Double = 1.0 / 1000

        private val LOG: Logger = LoggerFactory.getLogger(PerformanceTest::class.java)
    }

    @Test
    fun runfmi4j() {

        Fmu.from(testFmu).use { fmu ->

            fmu.asCoSimulationFmu().newInstance().use { slave ->

                val elapsed = measureTimeMillis {

                    slave.simpleSetup()
                    while (slave.simulationTime <= stop) {
                        slave.doStep(stepSize)
                        slave.variableAccessor.readReal(47)
                    }
                }

                LOG.info("FMI4j: $elapsed ms")

            }

        }

    }

    @Test
    fun runcse() {

        CseExecution.create(stepSize).use { execution ->

            val slave = execution.addSlave(testFmu)
            val observer = execution.addMembufferObserver()

            val numSteps = (stop / stepSize).toLong()

            val vr = longArrayOf(47)
            val ref = DoubleArray(1)
            val elapsed = measureTimeMillis {
                for (i in 0..numSteps) {
                    execution.step(1)
                    observer.getReal(slave, vr, ref)
                }
            }

            LOG.info("cse-core4j: $elapsed ms")

        }

    }

}

