package org.osp.cse

import no.ntnu.ihb.fmi4j.Fmi2Slave
import no.ntnu.ihb.fmi4j.ScalarVariable
import no.ntnu.ihb.fmi4j.modeldescription.fmi2.Fmi2Causality
import org.junit.jupiter.api.Test
import java.io.File

class ScenarioTest {

    @Test
    fun testJsonScenario() {

        val scenarioFile = File(ScenarioTest::class.java.classLoader.getResource("scenario/scenario1.json").file)

        CseExecution.create(1.0/100).use {

            it.addJvmSlave(Slave(), "slave uno")
            it.loadScenario(scenarioFile)

        }

    }

    @Test
    fun testYamlScenario() {

        val scenarioFile = File(ScenarioTest::class.java.classLoader.getResource("scenario/scenario1.yml").file)

        CseExecution.create(1.0/100).use {

            it.addJvmSlave(Slave(), "slave uno")
            it.loadScenario(scenarioFile)

        }

    }


    class Slave: Fmi2Slave() {

        @ScalarVariable(Fmi2Causality.input)
        var realIn: Double = 0.0

        @ScalarVariable(Fmi2Causality.output)
        var realOut: Double = 0.0

        override fun doStep(currentTime: Double, dt: Double): Boolean {
            return true
        }
    }

}
