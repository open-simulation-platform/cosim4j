package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File

class CseExecutionTest {

    private companion object {
        val test_fmu = File(CseExecutionTest::class.java.classLoader
            .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file)
    }

    @Test
    fun test1() {

        CseExecution(1.0/100).use { execution ->

            execution.setMemBufferObserver()

            val slave = execution.addSlave(test_fmu)

            Assertions.assertTrue(execution.start())
            Assertions.assertTrue(execution.step(1))

            println(slave.getReal(47))

            Assertions.assertTrue(execution.stop())


        }

    }

}