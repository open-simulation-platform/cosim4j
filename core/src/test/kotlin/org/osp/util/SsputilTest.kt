package org.osp.util

import org.junit.jupiter.api.Test
import org.junit.jupiter.api.Assertions.*

internal class SsputilTest {

    @Test
    fun extractStepSize() {

        assertEquals(0.05, extractStepSize(testSsp))

    }

    private val testSsp = """
        
        <?xml version="1.0" encoding="UTF-8"?>
        <ssd:SystemStructureDescription name="KPN Twinship Gunnerus case" version="1.0"
                                        xmlns:ssc="http://ssp-standard.org/SSP1/SystemStructureCommon"
                                        xmlns:ssd="http://ssp-standard.org/SSP1/SystemStructureDescription"
                                        xmlns:ssv="http://ssp-standard.org/SSP1/SystemStructureParameterValues">
        
          <ssd:System name="gunnerus" description="Gunnerus case">
        
              <ssd:Annotations>
                  <ssc:Annotation type="org.open-simulation-platform">
                      <osp:SimulationInformation>
                          <osp:FixedStepMaster description="FixedStepAlgorithm" stepSize="0.05"/>
                      </osp:SimulationInformation>
                  </ssc:Annotation>
              </ssd:Annotations>
        
          </ssd:System>
        
        </ssd:SystemStructureDescription>

    """.trimIndent()


}
