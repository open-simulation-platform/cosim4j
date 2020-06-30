package com.opensimulationplatform.cosim

import no.ntnu.ihb.fmi4j.export.fmi2.Fmi2Slave
import no.ntnu.ihb.fmi4j.export.fmi2.ScalarVariable
import no.ntnu.ihb.fmi4j.modeldescription.fmi2.Fmi2Causality
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.condition.DisabledOnOs
import org.junit.jupiter.api.condition.OS

class TestJvmSlave {

    @Test
    @DisabledOnOs(OS.LINUX)
    fun testJvmSlave() {

        CosimExecution.create(1.0 / 100).use { execution ->

            val observer = execution.addLastValueObserver()

            val slave = MySlave("slave1")
            val slaveRef = execution.addJvmSlave(slave).slaveRef

            val numSteps = 10L
            execution.step(numSteps)

            Assertions.assertEquals(1.0, observer.getReal(slaveRef, slave.getValueReference("speed")))
            Assertions.assertEquals(10, observer.getInteger(slaveRef, slave.getValueReference("count")))

        }

    }

}

internal class MySlave(
    instanceName: String
) : Fmi2Slave(mapOf("instanceName" to instanceName)) {

    @ScalarVariable(causality = Fmi2Causality.output)
    internal var speed = 1.0

    @ScalarVariable(causality = Fmi2Causality.output)
    internal var count = 0

    override fun doStep(currentTime: Double, dt: Double) {
        count += 1
    }

}
