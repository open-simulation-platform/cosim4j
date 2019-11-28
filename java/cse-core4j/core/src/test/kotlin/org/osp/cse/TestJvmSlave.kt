package org.osp.cse

import no.ntnu.ihb.fmi4j.export.fmi2.ScalarVariable
import no.ntnu.ihb.fmi4j.export.fmi2.Slave
import no.ntnu.ihb.fmi4j.modeldescription.fmi2.Fmi2Causality
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import java.io.File

class TestJvmSlave {

    @Test
    fun testJvmSlave() {

        CseExecution.create(1.0/100).use { execution ->

            val observer = execution.addLastValueObserver()

            val slave = MySlave("slave1")
            val index = execution.addJvmSlave(slave).index

            val numSteps = 10L
            execution.step(numSteps)

            Assertions.assertEquals(1.0, observer.getReal(index, slave.getValueReference("speed")))
            Assertions.assertEquals(10, observer.getInteger(index,slave.getValueReference("count")))

        }

    }

    @Test
    @Disabled
    fun testFmu() {

        CseExecution.create(1.0 / 100).use { exec ->

            exec.addSlave(File("D:\\Development\\FMI4j\\java\\fmi4j\\fmu-builder\\Test.fmu"), "test1")
            exec.addSlave(File("D:\\Development\\FMI4j\\java\\fmi4j\\fmu-builder\\Test.fmu"), "test2")

            exec.start()

            Thread.sleep(100)

            exec.stop()

        }

    }

}

internal class MySlave(
        instanceName: String
): Slave(instanceName) {

    @ScalarVariable(Fmi2Causality.output)
    internal var speed = 1.0

    @ScalarVariable(Fmi2Causality.output)
    internal var count = 0

    override fun doStep(currentTime: Double, dt: Double) {
        count += 1
    }

}
