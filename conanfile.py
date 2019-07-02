
from conans import ConanFile, CMake


class CsecorejniConan(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = (
        "boost/1.66.0@conan/stable"
    )

    default_options = (
        "boost:shared=True"
    )

    def imports(self):
        self.copy("*.dll", dst="bin", keep_path=False)
