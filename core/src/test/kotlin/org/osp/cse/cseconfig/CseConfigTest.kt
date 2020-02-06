package org.osp.cse.cseconfig

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Disabled
import org.junit.jupiter.api.Test
import org.osp.cse.CseExecution
import java.io.File

class CseConfigTest {

    @Test
    @Disabled
    fun TestCseConfig() {

        val path = File(CseConfigTest::class.java.classLoader.getResource("msmi")!!.file)
        CseExecution.createFromCseConfig(path).use {
           Assertions.assertEquals(it.getSlaveByReference(0).instanceName, "CraneController")
        }

    }
}
