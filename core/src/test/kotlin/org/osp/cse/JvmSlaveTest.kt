package org.osp.cse

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class JvmSlaveTest {

    @Test
    fun test() {

        CseExecution.create(1.0 / 100).use { execution ->

            val slave = MySlave()
            execution.addSlave(slave)

            val observer = execution.addLastValueObserver()!!

            val numSteps = 10L
            execution.step(numSteps)

            Assertions.assertEquals(numSteps.toDouble(), slave.realOut)
            Assertions.assertEquals(numSteps.toDouble(), observer.getReal(0, 0))

        }

    }

}

class MySlave : CseJvmSlave() {

    internal var realOut = 0.0

    override fun define(): Model {
        return Model("TestSlave")
                .add(real("realOut", { realOut }))
    }

    override fun setup(startTime: Double) {
        println("setup: startTime=$startTime")
    }

    override fun doStep(currentTime: Double, stepSize: Double) {
        realOut += 1
    }

    override fun terminate() {
        println("terminate")
    }

}
