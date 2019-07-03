# cse-core4j

High-level, easy to use Java bindings for cse-core, written in Kotlin.

The JNI code is written by hand in clean, easy to maintain C++, no magic involved. 


#### API

```java
class Demo {

    public static void main(String[] args) {

        try(CseExecution execution = CseExecution.createFromSsp(new File("path/to/sspDir"))) {

            //execution.enableRealTimeSimulation();
            //execution.disableRealTimeSimulation();

            CseLastValueObserver observer = execution.addLastValueObserver();

            execution.addFileObserver(new File("results"));
            execution.loadScenario(new File("path/to/scenario.json"));

            CseSlave slave1 = execution.addSlave(new File("path/to/fmu1.fmu"));
            CseSlave slave2 = execution.addSlave(new File("path/to/fmu2.fmu"));

            execution.connectReals(slave1, 12 /*vr*/, slave2, 9 /*vr*/);

            //execution.start();
            execution.step(10);
            //execution.stop();

            CseExecutionStatus status = execution.getStatus();
            double currentTime = status.getCurrentTime();
            CseExecutionState state = status.getState();
            CseErrorCode errorCode = status.getErrorCode();

            double value = slave1.getReal(observer, 46 /*vr*/);

            CseRealSamples samples = slave1.getRealSamples(observer, 46 /*vr*/, 0, 5);
            double[] values = samples.getValues();
            double[] times = samples.getTimes();
            long[] steps = samples.getSteps();

        } // try with resources (calls destroy internally)

    }

}
```

#### FatJar

It's easy to create a FatJar, which is a self-contained .jar file: 
```bash
./gradlew shadow
```
This will generate a JAR named _cse_core_shadow_\<version\>.jar into _build/libs/_

#### Maven local

If you want to generate a maven artifact and put it into your local maven repository (`.m2`), just run:
```bash
./gradlew publishToMavenLocal
```