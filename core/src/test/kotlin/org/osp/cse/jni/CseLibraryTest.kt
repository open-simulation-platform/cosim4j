package org.osp.cse.jni

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.osp.cse.TestFMUs


class CseLibraryTest {

    private companion object {
        val testFmu = TestFMUs.fmi20().cs()
            .vendor("20sim").version("4.6.4.8004")
            .name("ControlledTemperature").file().absolutePath
    }

    @Test
    fun testLibrary1() {

        val execution = CseLibrary.createExecution(0.0, 1.0/100)
        Assertions.assertTrue(CseLibrary.destroyExecution(execution))

    }

    @Test
    fun testLibrary2() {

        val execution = CseLibrary.createExecution(0.0, 1.0/100)

        val slave = CseLibrary.createLocalSlave(testFmu)
        val index = CseLibrary.addSlave(execution, slave)
        Assertions.assertEquals(0, index)

        Assertions.assertTrue(CseLibrary.step(execution, 1))
        Assertions.assertTrue(CseLibrary.destroyExecution(execution))

    }

}