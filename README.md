# cse-core4j

High-level, easy to use Java bindings for cse-core, written in Kotlin.

The JNI code is written by hand in clean, easy to maintain C++, no magic involved. 


#### API

```java
class Demo {
    
    public static void main(String[] args) {
        
        try(CseExecution execution = CseExecution.create(1.0/100)) {
            
            CseMembufferObserver membufferObserver = execution.addMembufferObserver();
            CseFileObserver fileObserver = execution.addFileObserver(new File("results"));
                    
            CseSlave slave1 = execution.addSlave("path/to/fmu1.fmu");
            CseSlave slave2 = execution.addSlave("path/to/fmu2.fmu");
            
            execution.connectReals(slave1, 12 /*vr*/, slave2, 9 /*vr*/);
            
            //execution.start();
            execution.step(1);
            //execution.stop();
            
            double value = membufferObserver.getReal(slave1, 46 /*vr*/);
            
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

If you want to generate a maven artifact and mut it into your local maven repository (`.m2`), just run:
```bash
./gradlew publishToMavenLocal
```