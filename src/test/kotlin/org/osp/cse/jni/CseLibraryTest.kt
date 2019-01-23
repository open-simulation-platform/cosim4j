package org.osp.cse.jni

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.time.Duration

class CseLibraryTest {


    @Test
    fun testLibrary() {

        CseLibrary().apply {

            val execution = createExecution(0, Duration.ofSeconds(1/100).toNanos())
            Assertions.assertTrue(destroyExecution(execution))
        }

    }

}