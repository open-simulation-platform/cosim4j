package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class JvmSlaveTest {

    @Test
    fun test() {

        val slaves = List(2) {i ->MySlave(i)}

        CseExecution.create(1.0 / 100).use { execution ->

            val observer = execution.addLastValueObserver()!!
            val manipulator = execution.addOverrideManipulator()!!

            val intInVr = 0L
            val intInWriteValue = 100

            val realInVr = 2L
            val realInWriteValue = 100.0

            slaves.forEach {
                val index = execution.addSlave((it))!!.index
                manipulator.setInteger(index, intInVr, intInWriteValue)
                manipulator.setReal(index, realInVr, realInWriteValue)
            }

            Assertions.assertEquals(slaves.size, execution.numSlaves)

            val numSteps = 10L
            execution.step(numSteps)

            slaves.forEachIndexed { slaveIndex, slave ->

                Assertions.assertEquals(slave.intOut, observer.getInteger(slaveIndex, 1L))
                Assertions.assertEquals(slave.realOut, observer.getReal(slaveIndex, 3L))

                Assertions.assertEquals(slave.intIn, observer.getInteger(slaveIndex, intInVr))
                Assertions.assertEquals(slave.realIn, observer.getReal(slaveIndex, realInVr))
            }

        }

    }

}

class MySlave(
        private val nr: Int
) : CseJvmSlave() {

    internal var intIn = 0
    internal var intOut = 0

    internal var realIn = 0.0
    internal var realOut = 0.0

    override fun define(): Model {
        return model("TestSlave")
                .description("Slave for testing")
                .author("Lars Ivar Hatledal")
                .add(integer("intIn", { intIn }, { intIn = it }).causality(CseVariableCausality.INPUT))
                .add(integer("intOut", { intOut }).causality(CseVariableCausality.OUTPUT))
                .add(real("realIn", { realIn }).causality(CseVariableCausality.INPUT))
                .add(real("realOut", { realOut }).causality(CseVariableCausality.OUTPUT))
    }

    override fun setup(startTime: Double) {}

    override fun doStep(currentTime: Double, stepSize: Double) {
        intOut += nr
        realOut += nr
    }

    override fun terminate() {
        println("terminate")
    }

}

class MySlave2 : CseJvmSlave() {

    internal var intIn = 0
    internal var intOut = 0
    internal var realOut1 = 0.0
    internal var realOut2 = 0.0

    override fun define(): Model {
        return Model("TestSlave2")
                .description("Slave for testing")
                .author("Lars Ivar Hatledal")
                .add(integer("intIn", { intIn }, { intIn = it }).causality(CseVariableCausality.INPUT))
                .add(integer("intOut", { intOut }).causality(CseVariableCausality.OUTPUT))
                .add(real("realOut1", { realOut1 }).causality(CseVariableCausality.OUTPUT))
                .add(real("realOut2", { realOut2 }).causality(CseVariableCausality.OUTPUT))
    }

    override fun setup(startTime: Double) {}

    override fun doStep(currentTime: Double, stepSize: Double) {
        intOut += 1
        realOut1 += 1
        realOut2 += 2
    }

    override fun terminate() {
        println("terminate")
    }

}
