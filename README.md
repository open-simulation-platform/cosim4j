# cse-core4j
cse-core JVM interface


```java

class Demo {
    
    public static void main(String[] args) {
        
        try(CseExecution execution = new CseExecution(1.0/100)) {
            execution.setMemBufferObserver();
                    
            CseSlave slave = execution.addSlave("path/to/fmu.fmu");
            
            execution.start();
            execution.step(10);
            
            double value = slave.getReal(0 /*vr*/);
            
            execution.stop();
        }
  
    }
    
}

```