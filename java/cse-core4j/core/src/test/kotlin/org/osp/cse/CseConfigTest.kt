package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File

class CseConfigTest {

    @Test
    fun TestCseConfig() {

        val path = File(CseConfigTest::class.java.classLoader.getResource("msmi").file)

        CseExecution.createFromCseConfig(path).use {

           Assertions.assertEquals(it.getSlave(0).name, "CraneController")

        }

    }
}