from conans import ConanFile, tools
import os


class BoostConan(ConanFile):
    name = "boost"
    version = "1.66.0"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    description = "Boost provides free peer-reviewed portable C++ source libraries"
    url = "https://github.com/odant/conan-boost"
    settings = "os", "compiler", "build_type", "arch"
    #------ internal ------
    _boost_name = "boost_%s" % version.replace(".", "_")
    _boost_archive = _boost_name + ".tar.gz"
    #------ internal ------
    exports_sources = _boost_archive, "FindBoost.cmake"
    no_copy_source = True
    build_policy = "missing"
    short_paths = True

    def source(self):
        self.output.info("-------------- Unzip sources --------------------")
        self.output.info("Current directory => %s" % os.getcwd())
        tools.rmdir(self._boost_name)
        tools.unzip(self._boost_archive)
        os.remove(self._boost_archive)
        
    def build(self):
        self.output.info("-------------- Bootstrap ------------------------")
        b2 = self.bootstrap()
        self.run("%s -v" % b2)
        self.output.info("-------------- user-config.jam ------------------")
        self.generate_user_config_jam()
        self.output.info("-------------- Build libraries ------------------")
        flags = self.get_build_flags()
        boost_source_folder = os.path.join(self.source_folder, self._boost_name)
        with tools.chdir(boost_source_folder):
            with tools.vcvars(self.settings) if self.settings.compiler == "Visual Studio" else tools.no_op():
            # location user-config.jam
                with tools.environment_append({"BOOST_BUILD_PATH": os.path.join(self.build_folder, "build")}):
                    self.output.info("Current directory => %s" % os.getcwd())
                    self.run("%s %s stage" % (b2, " ".join(flags)))
        
    def bootstrap(self):
        boost_source_folder = os.path.join(self.source_folder, self._boost_name)
        with tools.chdir(boost_source_folder):
            cmd = "bootstrap.bat" if tools.os_info.is_windows else "./bootstrap.sh"
            self.output.info("Current directory => %s" % os.getcwd())
            self.run(cmd)
        b2_exe = "b2.exe" if tools.os_info.is_windows else "b2"
        return os.path.join(boost_source_folder, b2_exe)

    def get_build_flags(self):
        flags = ["-a -d2 --debug-configuration --debug-generator --abbreviate-paths --build-type=minimal"]
        flags.append("--build-dir=%s" % os.path.join(self.build_folder, "build"))
        flags.append("--stagedir=%s" % os.path.join(self.build_folder, "stage"))
        flags += self.get_libraries_list()
        #flags.append("toolset=msvc-14.1")
        flags.append("link=static")
        flags.append("runtime-link=shared")
        flags.append("variant=debug")
        flags.append("address-model=64")
        return flags

    def get_libraries_list(self):
        libs = [
            "--with-system"
        ]
        return libs
        
    def generate_user_config_jam(self):
        content = ""
        compiler, version, compiler_exe = self.get_toolset()
        compiler_flags = self.get_compiler_flags()
        content += "using %s : %s : %s : %s;\n" % (compiler, version, compiler_exe, compiler_flags)
        self.output.info("Using current user-config.jam:\n%s" % content)
        fname = os.path.join(self.build_folder, "build", "user-config.jam")
        tools.save(fname, content)
        
    def get_toolset(self):
        return "msvc", "14.1", "cl.exe"
        
    def get_compiler_flags(self):
        return ""
        
    def package(self):
        self.copy("FindBoost.cmake", dst=".", src=".")
        self.copy(pattern="*", src="%s/boost" % self._boost_name, dst="include/boost")
        self.copy("*.lib", src="stage", dst="lib", keep_path=False)
        self.copy("*.a", src="stage", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.defines.append("BOOST_USE_STATIC_LIBS")
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            # DISABLES AUTO LINKING! NO SMART AND MAGIC DECISIONS THANKS!
            self.cpp_info.defines.extend(["BOOST_ALL_NO_LIB"])
