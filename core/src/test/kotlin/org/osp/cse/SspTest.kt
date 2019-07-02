package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import java.io.File

class SspTest {

    @Test
    fun testSsp() {
        testSsp(File(SspTest::class.java.classLoader.getResource("ssp/demo").file))
    }

    @Test
    @Disabled
    fun testSspFmuproxy() {
        testSsp(File(SspTest::class.java.classLoader.getResource("ssp/demo/fmuproxy").file))
    }

    fun testSsp(sspDir: File) {

        Assertions.assertTrue(sspDir.exists())

        CseExecution.createFromSsp(sspDir).use { execution ->

            execution.addFileObserver(File("build/results"))

            val numSteps = 100L
            Assertions.assertTrue(execution.step(numSteps))

            execution.getStatus().also {
                Assertions.assertEquals(numSteps*1e-4, it.currentTime)
            }

        }
    }

}