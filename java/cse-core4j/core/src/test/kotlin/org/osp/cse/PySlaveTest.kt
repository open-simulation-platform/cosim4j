package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.io.File

class PySlaveTest {

    @Test
    fun testPySlave() {

        val pyFile = File(PySlaveTest::class.java.classLoader.getResource("py/cseslave.py").file)

        CseExecution.create(1.0/100).use {

            val slave = it.addPySlave(pyFile, "pySlave")
            Assertions.assertEquals("CseSlave", slave.modelDescription.name)
            Assertions.assertEquals("Lars Ivar Hatledal", slave.modelDescription.author)

//            val variableRef = slave.getVariable("realOut").valueReference

//            val observer = it.addLastValueObserver()

//            it.step()

//            Assertions.assertEquals(5.0, observer.getReal(slave.index, variableRef))


        }

    }

}
