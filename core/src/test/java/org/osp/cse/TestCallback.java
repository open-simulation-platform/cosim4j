package org.osp.cse;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

public class TestCallback {

    @Test
    void test() throws InterruptedException {

        File testFmu = new File(CseExecutionTest.class.getClassLoader()
                .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").getFile());

        CseExecution execution = CseExecution.create(1.0 / 100);

        execution.addSlave(testFmu);

        AtomicInteger counter = new AtomicInteger(0);

        execution.addStepEventListener(() -> counter.incrementAndGet());

        int numSteps = 100;
        execution.step(numSteps);

        Assertions.assertEquals(numSteps, counter.get());

        execution.start();

        Thread.sleep(10);

        execution.stop();

        Assertions.assertTrue(numSteps < counter.get());

        execution.close();

    }

}
