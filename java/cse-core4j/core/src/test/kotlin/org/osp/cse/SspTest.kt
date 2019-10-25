package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import org.osp.util.extractStepSize
import java.io.File

class SspTest {

    @Test
    fun testSsp() {
        val sspDir = File(SspTest::class.java.classLoader.getResource("ssp/demo").file)
        testSsp(sspDir)
        testSsp2(sspDir)
    }

    @Test
    @Disabled
    fun testSspFmuproxy() {
        testSsp(File(SspTest::class.java.classLoader.getResource("ssp/demo/fmuproxy").file))
    }

    fun testSsp(sspDir: File) {

        Assertions.assertTrue(sspDir.exists())

        CseExecution.createFromSsp(sspDir).use { execution ->

            val craneController = execution.getSlave("CraneController")
            Assertions.assertNotNull(craneController)
            val variable = craneController.modelDescription.getVariable("Gain1.input")
            Assertions.assertNotNull(variable)
            Assertions.assertEquals(154, variable!!.valueReference)
            Assertions.assertEquals(CseVariableType.REAL, variable.type)
            Assertions.assertEquals(CseVariableCausality.LOCAL, variable.causality)

            execution.addFileObserver(File("build/results"))

            val numSteps = 100L
            Assertions.assertTrue(execution.step(numSteps))

            execution.status.also {
                Assertions.assertEquals(numSteps * extractStepSize(sspDir)!!, it.currentTime)
            }

        }
    }

    fun testSsp2(sspDir: File) {

        Assertions.assertTrue(sspDir.exists())

        val stepSize = 1.0 / 100
        CseExecution.createFromSsp(sspDir, stepSize = stepSize).use { execution ->

            val numSteps = 100L
            Assertions.assertTrue(execution.step(numSteps))

            execution.status.also {
                Assertions.assertEquals(numSteps * stepSize, it.currentTime)
            }

        }
    }

}
