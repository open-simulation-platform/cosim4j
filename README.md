# cse-core4j
cse-core JVM interface


```java

class Demo {
    
    public static void main(String[] args) {
        
        try(CseExecution execution = new CseExecution(1.0/100)) {
            execution.setMemBufferObserver();
                    
            CseSlave slave1 = execution.addSlave("path/to/fmu1.fmu");
            CseSlave slave2 = execution.addSlave("path/to/fmu2.fmu");
            
            execution.connectReals(slave1, 12 /*vr*/, slave2, 9 /*vr*/);
            
            //execution.start();
            execution.step(10);
            //execution.stop();
            
            double value = slave1.getReal(46 /*vr*/);
            
        }
  
    }
    
}

```