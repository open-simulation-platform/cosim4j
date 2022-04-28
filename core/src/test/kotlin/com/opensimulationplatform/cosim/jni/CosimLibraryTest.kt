package com.opensimulationplatform.cosim.jni

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File


class CosimLibraryTest {

    private companion object {
        val cl = CosimLibraryTest::class.java.classLoader
        val testFmu = File(
            cl.getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file.replace(
                "%20",
                " "
            )
        )
    }

    @Test
    fun testLibrary1() {

        val execution = CosimLibrary.createExecution(0.0, 1.0 / 100)
        Assertions.assertTrue(CosimLibrary.destroyExecution(execution))

    }

    @Test
    fun testLibrary2() {

        Assertions.assertTrue(testFmu.exists())

        val execution = CosimLibrary.createExecution(0.0, 1.0 / 100)

        val slave = CosimLibrary.createSlave(testFmu.absolutePath, "slave")
        val index = CosimLibrary.addSlave(execution, slave)
        Assertions.assertEquals(0, index)

        Assertions.assertTrue(CosimLibrary.step(execution, 1))
        Assertions.assertTrue(CosimLibrary.destroyExecution(execution))

    }

}
