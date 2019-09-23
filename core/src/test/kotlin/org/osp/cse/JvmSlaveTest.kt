package org.osp.cse

import org.junit.jupiter.api.Test

class JvmSlaveTest {
    
    @Test
    fun test() {
        
    }
    
}

class Slave: JvmSlave("TestSlave") {

    private val realOut = 0.0
    
    override fun enterInitialisationMode(startTime: Double) {
        registerVariables(
                RealVar("realOut", {realOut})
        )
    }

    override fun exitInitialisationMode() {
        
    }

    override fun doStep(currentTime: Double, stepSize: Double): Boolean {

        return true
    }

    override fun terminate() {
        
    }

}
