package org.osp.cse

import org.junit.jupiter.api.Test
import java.io.File

class PySlaveTest {

    @Test
    fun testPySlave() {

        CseExecution.create(1.0/100).use {

            val slave = it.addPySlave(File("C:/Users/LarsIvar/Documents/IdeaProjects/PythonFMU/cseslave.py"), "pySlave")
            println(slave.modelDescription.name)

        }

    }

}
