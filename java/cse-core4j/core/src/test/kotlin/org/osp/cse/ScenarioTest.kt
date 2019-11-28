package org.osp.cse

import no.ntnu.ihb.fmi4j.export.fmi2.ScalarVariable
import no.ntnu.ihb.fmi4j.export.fmi2.Slave
import no.ntnu.ihb.fmi4j.modeldescription.fmi2.Fmi2Causality
import org.junit.jupiter.api.Test
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File

class ScenarioTest {

    companion object {
        private val LOG: Logger = LoggerFactory.getLogger(ScenarioTest::class.java)
    }

    @Test
    fun testJsonScenario() {

        val scenarioFile = File(ScenarioTest::class.java.classLoader.getResource("scenario/scenario1.json")!!.file)

        CseExecution.create(1.0/100).use {

            val slave = it.addJvmSlave(MySlave("slave uno"))
            LOG.info("${slave.modelDescription.variables}")
            it.loadScenario(scenarioFile)

        }

    }

    @Test
    fun testYamlScenario() {

        val scenarioFile = File(ScenarioTest::class.java.classLoader.getResource("scenario/scenario1.yml")!!.file)

        CseExecution.create(1.0/100).use {

            val slave = it.addJvmSlave(MySlave("slave uno"))
            LOG.info("${slave.modelDescription.variables}")
            it.loadScenario(scenarioFile)

        }

    }


    class MySlave(
            instanceName: String
    ): Slave(instanceName) {

        @ScalarVariable(Fmi2Causality.input)
        var realIn: Double = 0.0

        @ScalarVariable(Fmi2Causality.output)
        var realOut: Double = 0.0

        override fun doStep(currentTime: Double, dt: Double) {}
    }

}
