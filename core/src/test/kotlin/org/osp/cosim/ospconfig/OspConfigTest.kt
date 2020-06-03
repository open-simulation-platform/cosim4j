package org.osp.cosim.ospconfig

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import org.osp.cosim.CosimExecution
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
