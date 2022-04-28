package com.opensimulationplatform.cosim

import com.opensimulationplatform.cosim.ssp.SspTest
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File

class RealTimeExecutionTest {

    @Test
    fun testRealTime() {

        val cl = SspTest::class.java.classLoader
        val sspDir = File(cl.getResource("ssp/demo")!!.file.replace("%20", " "))
        Assertions.assertTrue(sspDir.exists())
        CosimExecution.createFromSsp(sspDir).use {

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
