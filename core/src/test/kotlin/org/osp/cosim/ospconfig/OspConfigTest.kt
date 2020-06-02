package org.osp.cosim.ospconfig

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import org.osp.cosim.CosimExecution
import java.io.File

class OspConfigTest {

    @Test
    @Disabled
    fun TestOspConfig() {

        val path = File(OspConfigTest::class.java.classLoader.getResource("msmi")!!.file)
        CosimExecution.createFromOspConfig(path).use {
           Assertions.assertEquals(it.getSlaveByReference(0).instanceName, "CraneController")
        }

    }

}
