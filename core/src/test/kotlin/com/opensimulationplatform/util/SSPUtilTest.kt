package com.opensimulationplatform.util

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class SSPUtilTest {

    private fun testString(stepSize: String) = """
       <ssd:DefaultExperiment>
            <ssd:Annotations>
                <ssc:Annotation type="com.opensimulationplatform">
                    <osp:Algorithm>
                        <osp:FixedStepAlgorithm baseStepSize="$stepSize"/>
                    </osp:Algorithm>
                </ssc:Annotation>
            </ssd:Annotations>
        </ssd:DefaultExperiment>
    """.trimIndent()

    @Test
    fun extractStepSize() {
        listOf("0.001", "1E-4", "1.0E-4", "1e-4", "1.0E-4" ).forEach {
            Assertions.assertEquals(it.toDouble(), extractFixedStepAlgorithmStepSize(testString(it)))
        }
    }
}
