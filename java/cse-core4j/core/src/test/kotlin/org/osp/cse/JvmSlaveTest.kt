package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test


class JvmSlaveTest {

    @Test
    fun test() {

        val intInVr = 0L
        val intInWriteValue = 100

        val realInVr = 2L
        val realInWriteValue = 50.0

        val boolInVr = 4L
        val boolInWriteValue = true

        val strInVr = 6L
        val strInWriteValue = "Hello World!"

        val slaves = List(2) { MySlave() }

        CseExecution.create(1.0 / 100).use { execution ->

            val observer = execution.addLastValueObserver()!!
            val manipulator = execution.addOverrideManipulator()!!

            slaves.forEach {
                val index = execution.addSlave((it))!!.index
                manipulator.setInteger(index, intInVr, intInWriteValue)
                manipulator.setReal(index, realInVr, realInWriteValue)
                manipulator.setBoolean(index, boolInVr, boolInWriteValue)
                manipulator.setString(index, strInVr, strInWriteValue)
            }

            Assertions.assertEquals(slaves.size, execution.numSlaves)

            val numSteps = 10L
            execution.step(numSteps)

            slaves.forEachIndexed { slaveIndex, slave ->

                Assertions.assertEquals(slave.intOut, observer.getInteger(slaveIndex, 1L))
                Assertions.assertEquals(slave.realOut, observer.getReal(slaveIndex, 3L))
                Assertions.assertEquals(slave.boolOut, observer.getBoolean(slaveIndex, 5L))
                Assertions.assertEquals(slave.stringOut, observer.getString(slaveIndex, 7L))

                Assertions.assertEquals(intInWriteValue, observer.getInteger(slaveIndex, intInVr))
                Assertions.assertEquals(realInWriteValue, observer.getReal(slaveIndex, realInVr))
                Assertions.assertEquals(boolInWriteValue, observer.getBoolean(slaveIndex, boolInVr))
                Assertions.assertEquals(strInWriteValue, observer.getString(slaveIndex, strInVr))

                Assertions.assertEquals(9, observer.getInteger(slaveIndex, 8L))
                Assertions.assertEquals(9, observer.getInteger(slaveIndex, 9L))

                Assertions.assertEquals("Hello", observer.getString(slaveIndex, 10L))
                Assertions.assertEquals("world!", observer.getString(slaveIndex, 11L))

            }

        }

    }

}

@SlaveInfo(name = "SimpleSlave")
class MySlave : CseJvmSlave() {

    @ScalarVariable(causality = CseVariableCausality.INPUT)
    internal var intIn = 1
    @ScalarVariable(causality = CseVariableCausality.OUTPUT)
    internal var intOut = 2

    @ScalarVariable(causality = CseVariableCausality.INPUT)
    internal var realIn = 3.0
    @ScalarVariable(causality = CseVariableCausality.OUTPUT)
    internal var realOut = 4.0

    @ScalarVariable(causality = CseVariableCausality.INPUT)
    internal var boolIn = false
    @ScalarVariable(causality = CseVariableCausality.OUTPUT)
    internal var boolOut = true

    @ScalarVariable(causality = CseVariableCausality.INPUT)
    internal var stringIn = ""
    @ScalarVariable(causality = CseVariableCausality.OUTPUT)
    internal var stringOut = "Hello world!"

    @ScalarVariable(causality = CseVariableCausality.LOCAL)
    internal var ints = intArrayOf(9, 9)

    @ScalarVariable(causality = CseVariableCausality.LOCAL)
    internal var strings = arrayOf("Hello", "world!")

    override fun setup(startTime: Double) {}

    override fun doStep(currentTime: Double, stepSize: Double) {
        intOut += 1
        realOut += 1
    }

    override fun terminate() {
        println("terminate")
    }

}
