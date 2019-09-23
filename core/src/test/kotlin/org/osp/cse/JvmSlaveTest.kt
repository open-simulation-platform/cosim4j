package org.osp.cse

import org.junit.jupiter.api.Test

class JvmSlaveTest {

    @Test
    fun test() {

        CseExecution.create(1.0 / 100).use { execution ->

            execution.addSlave(MySlave())

            execution.step(10)

        }

    }

}

class MySlave : CseJvmSlave() {

    private var realOut = 0.0

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
