
from conans import ConanFile, CMake


class CsecorejniConan(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = (
        "cse-core/0.3.0@markaren/testing"
    )

    default_options = (
        "cse-core:build_apidoc=False",
        "cse-core:revision=link-libzip-statically"
    )

    def imports(self):
        self.copy("*.dll", dst="cse/bin", keep_path=False)
        self.copy("*.lib", dst="cse/lib", keep_path=False)
        self.copy("**/cse.h", dst="cse", keep_path=False)
