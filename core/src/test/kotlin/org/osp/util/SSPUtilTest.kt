package org.osp.util

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

import org.junit.jupiter.api.Assertions.*

class SSPUtilTest {

    private val test1 = """
        <ssd:Annotations>
            <ssc:Annotation type="org.open-simulation-platform">
                <osp:SimulationInformation>
                <osp:FixedStepMaster description="FixedStepAlgorithm" stepSize="0.01"/>
                </osp:SimulationInformation>
            </ssc:Annotation>
        </ssd:Annotations>
    """.trimIndent()

    private val test2 = """
        <ssd:Annotations>
            <ssc:Annotation type="org.open-simulation-platform">
                <osp:SimulationInformation>
                <osp:FixedStepMaster description="FixedStepAlgorithm" stepSize="1e-4"/>
                </osp:SimulationInformation>
            </ssc:Annotation>
        </ssd:Annotations>
    """.trimIndent()

    @Test
    fun extractStepSize() {

        Assertions.assertEquals(0.01, extractStepSize(test1))
        Assertions.assertEquals(1e-4, extractStepSize(test2))

    }
}