# Boost test for Conan package
# Dmitriy Vetutnev, Odant, 2018, 2020


from conans import ConanFile, CMake, tools


class PackageTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "ninja/1.9.0"

    def imports(self):
        self.copy("*.pdb", dst="bin", src="bin")
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.so*", dst="bin", src="lib")

    def build(self):
        cmake = CMake(self, generator="Ninja", msbuild_verbosity='normal')
        cmake.verbose = True
        cmake.configure()
        cmake.build()
        self.cmake_is_multi_configuration = cmake.is_multi_configuration

    def test(self):
        env = {}
        if self.settings.os != "Windows":
            env = {"LANG": "en_US.utf8"}
        with tools.environment_append(env):
            if self.cmake_is_multi_configuration:
                self.run("ctest --verbose --build-config %s" % self.settings.build_type)
            else:
                self.run("ctest --verbose")

