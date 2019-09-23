package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class JvmSlaveTest {

    @Test
    fun test() {

        CseExecution.create(1.0 / 100).use { execution ->

            val slave = MySlave()
            execution.addSlave(slave)

            val manipulator = execution.addOverrideManipulator()!!
            val observer = execution.addLastValueObserver()!!

            val numSteps = 10L
            execution.step(numSteps)

            var vr = 1L
            manipulator.setInteger(0, 0, 100)

            Assertions.assertEquals(numSteps.toInt(), slave.intOut)
            Assertions.assertEquals(numSteps.toInt(), observer.getInteger(0, vr++))

            Assertions.assertEquals(numSteps.toDouble(), slave.realOut1)
            Assertions.assertEquals(numSteps.toDouble(), observer.getReal(0, vr++))

            Assertions.assertEquals(numSteps.toDouble()*2, slave.realOut2)
            Assertions.assertEquals(numSteps.toDouble()*2, observer.getReal(0, vr++))

            execution.step(1)

            Assertions.assertEquals(100, slave.intIn)
            Assertions.assertEquals(100, observer.getInteger(0, 0L))


        }

    }

}

class MySlave : CseJvmSlave() {

    internal var intOut = 0
    internal var realOut1 = 0.0
    internal var realOut2 = 0.0

    internal var intIn = 0

    override fun define(): Model {
        return Model("TestSlave")
                .description("Slave for testing")
                .author("Lars Ivar Hatledal")
                .add(integer("intIn", {intIn}, {intIn = it }).causality(CseVariableCausality.INPUT))
                .add(integer("intOut", {intOut}).causality(CseVariableCausality.OUTPUT))
                .add(real("realOut1", { realOut1 }).causality(CseVariableCausality.OUTPUT))
                .add(real("realOut2", { realOut2 }).causality(CseVariableCausality.OUTPUT))
    }

    override fun setup(startTime: Double) {
        println("setup: startTime=$startTime")
    }

    override fun doStep(currentTime: Double, stepSize: Double) {
        intOut += 1
        realOut1 += 1
        realOut2 += 2
    }

    override fun terminate() {
        println("terminate")
    }

}
