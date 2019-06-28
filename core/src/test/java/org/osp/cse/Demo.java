package org.osp.cse;

import java.io.File;

class Demo {

    public static void main(String[] args) {

        if (true) {
            throw new IllegalStateException("Demo code. Not intended to be run!");
        }

        try(CseExecution execution = CseExecution.create(1.0/100)) {

            //execution.enableRealTimeSimulation();
            //execution.disableRealTimeSimulation();

            CseFileObserver fileObserver = execution.addFileObserver(new File("results"));

            CseSlave slave1 = execution.addSlave(new File("path/to/fmu1.fmu"));
            CseSlave slave2 = execution.addSlave(new File("path/to/fmu2.fmu"));

            CseSlaveInfo[] infos = execution.getSlaveInfos();
            String slave1Name = infos[0].getName();

            execution.connectReals(slave1, 12 /*vr*/, slave2, 9 /*vr*/);

            //execution.start();
            execution.step(10);
            //execution.stop();

            CseExecutionStatus status = execution.getStatus();
            double currentTime = status.getCurrentTime();
            CseExecutionState state = status.getState();
            CseErrorCode errorCode = status.getErrorCode();

        } // try with resources (calls destroy internally)

    }

}
