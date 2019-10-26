from fmi2slave import *


class CseSlave(Fmi2Slave):

    Fmi2Slave.modelName = "CseSlave"
    Fmi2Slave.author = "Lars Ivar Hatledal"
    
    def __init__(self):
        super().__init__()

        self.realOut = 5.0
        # self.register_variable(Real("realOut"))
        
    def do_step(self, current_time, step_size):
        return True
