package org.osp.cse;


@SlaveInfo(name = "SimpleSlave")
public class SimpleSlave extends CseJvmSlave {

    @ScalarVariable(causality = CseVariableCausality.INPUT)
    private double realIn = 0;

    @Override
    public void setup(double startTime) {
    }

    @Override
    public void doStep(double currentTime, double stepSize) {
    }

}
