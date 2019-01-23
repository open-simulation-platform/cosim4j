package org.osp.cse.jni

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.time.Duration

class CseLibraryTest {

    companion object {
        val test_fmu = CseLibraryTest::class.java.classLoader
            .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").file
    }

    @Test
    fun testLibrary1() {

        CseLibrary().apply {
            val execution = createExecution(0, Duration.ofSeconds(1/100).toNanos())
            Assertions.assertTrue(destroyExecution(execution))

        }

    }

    @Test
    fun testLibrary2() {

        CseLibrary().apply {
            val execution = createExecution(0, Duration.ofSeconds(1/100).toNanos())

            val slave = createLocalSlave(test_fmu)
            val index = addSlave(execution, slave)

            Assertions.assertTrue(step(execution, 1))

            Assertions.assertTrue(destroyExecution(execution))

        }

    }

}