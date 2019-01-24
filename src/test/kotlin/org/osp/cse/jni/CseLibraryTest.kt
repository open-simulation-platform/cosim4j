package org.osp.cse.jni

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test


class CseLibraryTest {

    private companion object {
        val test_fmu = CseLibraryTest::class.java.classLoader
            .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file
    }

    @Test
    fun testLibrary1() {

        val execution = CseLibrary.createExecution(0.0, 1.0/100)
        Assertions.assertTrue(CseLibrary.destroyExecution(execution))

    }

    @Test
    fun testLibrary2() {

        val execution = CseLibrary.createExecution(0.0, 1.0/100)

        val slave = CseLibrary.createLocalSlave(test_fmu)
        val index = CseLibrary.addSlave(execution, slave)
        Assertions.assertEquals(0, index)

        Assertions.assertTrue(CseLibrary.step(execution, 1))
        Assertions.assertTrue(CseLibrary.destroyExecution(execution))

    }

}