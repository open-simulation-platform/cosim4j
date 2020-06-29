package com.opensimulationplatform.cosim.ospconfig

import com.opensimulationplatform.cosim.CosimExecution
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File

class OspConfigTest {

    @Test
    fun TestOspConfig() {

        val path = File(OspConfigTest::class.java.classLoader.getResource("msmi")!!.file)
        CosimExecution.createFromOspConfig(path).use {
            Assertions.assertDoesNotThrow {
                it.getSlaveByName("CraneController")
            }
            Assertions.assertDoesNotThrow {
                it.getSlaveByName("KnuckleBoomCrane")
            }
        }

    }

}
