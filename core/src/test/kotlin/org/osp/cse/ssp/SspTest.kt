package org.osp.cse.ssp

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.osp.cse.CseExecution
import org.osp.cse.CseVariableCausality
import org.osp.cse.CseVariableType
import org.osp.util.extractFixedStepAlgorithmStepSize
import java.io.File

class SspTest {

    private companion object {
        val sspDir = File(SspTest::class.java.classLoader.getResource("ssp/demo")!!.file)
    }

//    @Test
//    @Disabled
//    fun testSspFmuproxy() {
//        testSsp(File(SspTest::class.java.classLoader.getResource("ssp/demo/fmuproxy").file))
//    }

    @Test
    fun testSsp() {

        Assertions.assertTrue(sspDir.exists())

        CseExecution.createFromSsp(sspDir).use { execution ->

            val craneController = execution.getSlaveByName("CraneController")
            Assertions.assertNotNull(craneController)
            val variable = craneController.modelDescription.getVariable("Gain1.input")
            Assertions.assertNotNull(variable)
            Assertions.assertEquals(154, variable.valueReference)
            Assertions.assertEquals(CseVariableType.REAL, variable.type)
            Assertions.assertEquals(CseVariableCausality.LOCAL, variable.causality)

            execution.addFileObserver(File("build/results"))

            val numSteps = 100L
            Assertions.assertTrue(execution.step(numSteps))

            execution.status.also {
                Assertions.assertEquals(numSteps * extractFixedStepAlgorithmStepSize(sspDir)!!, it.currentTime)
            }

        }
    }

    @Test
    fun testSsp2() {

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

    @Test
    fun testSspArchive() {

        val stepSize = 1.0 / 100
        CseExecution.createFromSsp(File(sspDir, "demo.ssp"), stepSize = stepSize).use { execution ->

            val numSteps = 100L
            Assertions.assertTrue(execution.step(numSteps))

            Assertions.assertNotNull(execution.getSlaveByName("CraneController"))

            execution.status.also {
                Assertions.assertEquals(numSteps * stepSize, it.currentTime)
            }

        }
    }

}
