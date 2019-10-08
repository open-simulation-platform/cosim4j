
from Fmi2Slave import Accessor, Fmi2Slave

class Model(Fmi2Slave):

    def __init__(self):
        super().__init__()
        self.realOut: float = 2.0
        self.accessors.append((lambda: self.realOut, lambda value: setattr(self, "realOut", value)))

    def doStep(self, currentTime: float, stepSize: float):
        print(f"doStep, currentTime={currentTime}, stepSize={stepSize}")
        self.accessors[0][1](3.0)
        print(f"{self.accessors[0][0]()}")
