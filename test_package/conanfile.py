# Boost test for Conan package
# Dmitriy Vetutnev, Odant, 2018


from conans import ConanFile, CMake, tools


class PackageTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def imports(self):
        self.copy("*.pdb", dst="bin", src="bin")
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.so*", dst="bin", src="lib")

    def build(self):
        cmake = CMake(self, msbuild_verbosity='normal')
        cmake.verbose = True
        cmake.configure()
        cmake.build()

    def test(self):
        env = {}
        if self.settings.os != "Windows":
            env = {"LANG": "en_US.utf8"}
        with tools.environment_append(env):
            if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
                self.run("ctest --verbose --build-config %s" % self.settings.build_type)
            else:
                self.run("ctest --verbose")
