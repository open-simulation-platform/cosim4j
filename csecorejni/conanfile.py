
from conans import ConanFile, CMake


class CsecorejniConan(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = (
        "cse-core/0.3.0@osp/master"
    )

    default_options = {
        "csecore:fmuproxy": False
    }

    def imports(self):
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.pdb", dst="bin", keep_path=False)
