package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File

class RealTimeExecutionTest {

    @Test
    fun testRealTime() {

        val sspDir = File(SspTest::class.java.classLoader.getResource("ssp/demo").file)
        Assertions.assertTrue(sspDir.exists())
        CseExecution.createFromSsp(sspDir).use {

            it.start()

            Assertions.assertFalse(it.status.realTimeSimulation)
            it.enableRealTimeSimulation()
            Assertions.assertTrue(it.status.realTimeSimulation)
            it.disableRealTimeSimulation()
            Assertions.assertFalse(it.status.realTimeSimulation)

            it.stop()

        }

    }

}
