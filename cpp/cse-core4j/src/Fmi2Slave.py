from abc import ABC, abstractmethod


class Accessor:

    def __init__(self, getter):
        self.getter = getter


class Fmi2Slave(ABC):

    def __init__(self):
        self.accessors = []

    def setupExperiment(self, startTime: float):
        members = [getattr(self,attr) for attr in dir(self) if not attr.startswith("__")]
        print(vars(self))
        # print(members)
        print(f"setupExperiment, startTime={startTime}")

    def enterInitializationMode(self):
        print("enterInitializationMode")

    def exitInitializationMode(self):
        print("exitInitializationMode")

    @abstractmethod
    def doStep(self, currentTime: float, stepSize: float):
        pass
