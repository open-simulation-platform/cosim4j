package org.osp.util

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class SSPUtilTest {

    private val step1 = "0.01"
    private val step2 = "1e-4"

    private val test1 = """
       <ssd:DefaultExperiment>
            <ssd:Annotations>
                <ssc:Annotation type="com.opensimulationplatform">
                    <osp:SimulationInformation>
                    <osp:FixedStepAlgorithm baseStepSize="$step1"/>
                    </osp:SimulationInformation>
                </ssc:Annotation>
            </ssd:Annotations>
        </ssd:DefaultExperiment>
    """.trimIndent()

    private val test2 = """
        <ssd:DefaultExperiment>
            <ssd:Annotations>
                <ssc:Annotation type="com.opensimulationplatform">
                    <osp:SimulationInformation>
                    <osp:FixedStepAlgorithm baseStepSize="$step2"/>
                    </osp:SimulationInformation>
                </ssc:Annotation>
            </ssd:Annotations>
        </ssd:DefaultExperiment>
    """.trimIndent()

    @Test
    fun extractStepSize() {

        Assertions.assertEquals(step1.toDouble(), extractFixedStepAlgorithmStepSize(test1))
        Assertions.assertEquals(step2.toDouble(), extractFixedStepAlgorithmStepSize(test2))

    }
}