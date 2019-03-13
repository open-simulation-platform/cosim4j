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


        private const val warmup = 3
        private const val stop: Double = 10.0
        private const val stepSize: Double = 1.0 / 1000
        private const val numSteps = (stop / stepSize).toLong()

        private const val numVarsToRead = 25
        private val vr = LongArray(numVarsToRead) {i -> i.toLong()}
        private val ref = DoubleArray(numVarsToRead)

        private val LOG: Logger = LoggerFactory.getLogger(PerformanceTest::class.java)
    }

    @Test
    fun runfmi4j() {

        Fmu.from(testFmu).use { fmu ->

            for (i in 0..warmup) {
                fmu.asCoSimulationFmu().newInstance().use { slave ->

                    slave.simpleSetup()
                    measureTimeMillis {
                        while (slave.simulationTime <= stop) {
                            slave.doStep(stepSize)
                        }
                    }.also { elapsed ->
                        if (i == warmup) {
                            LOG.info("FMI4j: $elapsed ms")
                        }
                    }

                }
            }

        }

    }

    @Test
    fun runfmi4jwithread() {

        Fmu.from(testFmu).use { fmu ->

            for (i in 0..warmup) {
            fmu.asCoSimulationFmu().newInstance().use { slave ->
                    slave.simpleSetup()
                    measureTimeMillis {
                        while (slave.simulationTime <= stop) {
                            slave.doStep(stepSize)
                            slave.variableAccessor.readReal(vr, ref)
                        }
                    }.also { elapsed ->
                        if (i == warmup) {
                            LOG.info("FMI4j[with read]: $elapsed ms")
                        }
                    }
                }
            }

        }

    }

    @Test
    fun runcse() {

        for (i in 0..warmup) {
        CseExecution.create(stepSize).use { execution ->
                execution.addSlave(testFmu)
                execution.step(1)
                measureTimeMillis {
                    for (j in 0 until numSteps) {
                        execution.step(1)
                    }
                }.also { elapsed ->
                    if (i == warmup) {
                        LOG.info("cse-core4j: $elapsed ms")
                    }
                }
            }
        }

    }

    @Test
    fun runcsewithobserver() {

        for (i in 0..warmup) {
        CseExecution.create(stepSize).use { execution ->
                val slave = execution.addSlave(testFmu)
                val observer = execution.addMembufferObserver()
                measureTimeMillis {
                    for (j in 0..numSteps) {
                        execution.step(1)
                        observer.getReal(slave, vr, ref)
                    }
                }.also { elapsed ->
                    if (i == warmup) {
                        LOG.info("cse-core4j[with read]: $elapsed ms")
                    }
                }
            }
        }

    }

}

