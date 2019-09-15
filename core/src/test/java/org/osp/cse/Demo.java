package org.osp.cse;

import java.io.File;

class Demo {

    public static void main(String[] args) {

        if (true) {
            throw new IllegalStateException("Demo code. Not intended to be run!");
        }

        try(CseExecution execution = CseExecution.createFromSsp(new File("path/to/sspDir"))) {

            CseSlave mySlave = execution.getSlave("mySlave");
            int index = mySlave.getIndex();

            CseVariableDescription myVar = mySlave.getModelDescription().getVariable("myVar");
            long vr = myVar.getValueReference();

            execution.addFileObserver(new File("results"));
            execution.loadScenario(new File("path/to/scenario.json"));

            CseSlave slave1 = execution.addSlave(new File("path/to/fmu1.fmu"));
            CseSlave slave2 = execution.addSlave(new File("path/to/fmu2.fmu"));

            execution.connectReals(slave1.getIndex(), 12 /*vr*/, slave2.getIndex(), 9 /*vr*/);

            CseLastValueObserver observer = execution.addLastValueObserver();
            execution.step(10);
            double value = observer.getReal(slave1.getIndex(), 46 /*vr*/);

        } // try with resources (calls destroy internally)

    }

}
