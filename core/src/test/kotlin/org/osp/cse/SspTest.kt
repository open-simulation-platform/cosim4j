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

            val craneController = execution.getSlave("CraneController")
            Assertions.assertNotNull(craneController)
            val variable = craneController!!.modelDescription.getVariable("Gain1.input")
            Assertions.assertNotNull(variable)
            Assertions.assertEquals(154, variable!!.valueReference)
            Assertions.assertEquals(CseVariableType.REAL, variable.type)
            Assertions.assertEquals(CseVariableCausality.LOCAL, variable.causality)

            execution.addFileObserver(File("build/results"))

            val numSteps = 100L
            Assertions.assertTrue(execution.step(numSteps))

            execution.status!!.also {
                Assertions.assertEquals(numSteps*1e-4, it.currentTime)
            }

        }
    }

}
