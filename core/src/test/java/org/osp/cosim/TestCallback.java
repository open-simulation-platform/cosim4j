package org.osp.cosim;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

public class TestCallback {

    @Test
    void test() throws InterruptedException {

        File testFmu = new File(CosimExecutionTest.class.getClassLoader()
                .getResource("fmus/2.0/cs/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu").getFile());

        CosimExecution execution = CosimExecution.create(1.0 / 100);
        execution.addSlave(testFmu, "slave");

        AtomicInteger counter = new AtomicInteger(0);
        execution.addStepEventListener(new StepEventAdapter() {
            @Override
            public void onStepComplete(long stepNumber, double currentTime, double lastStepSize) {
                Assertions.assertEquals(counter.incrementAndGet(), stepNumber);
            }
        });

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
