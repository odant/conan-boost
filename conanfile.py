from conans import ConanFile, tools
import os


class BoostConan(ConanFile):
    name = "boost"
    version = "1.66.0"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    description = "Boost provides free peer-reviewed portable C++ source libraries"
    url = "https://github.com/odant/conan-boost"
    settings = {
        "os": ["Windows", "Linux"],
        "compiler": ["Visual Studio", "gcc"],
        "build_type": ["Debug"],
        "arch": ["x86_64", "x86"]
    }
    options = {
        "fPIC": [True, False]
    }
    default_options = "fPIC=True"
    #------ internal ------
    _boost_name = "boost_%s" % version.replace(".", "_")
    _boost_archive = _boost_name + ".tar.gz"
    _zlib_version = "1.2.11"
    #------ internal ------
    exports_sources = _boost_archive, "FindBoost.cmake"
    no_copy_source = True
    build_policy = "missing"
    short_paths = True

    def configure(self):
        if self.settings.os == "Windows":
            self.options.fPIC = False

    def requirements(self):
        self.requires("zlib/%s@%s/stable" % (self._zlib_version, self.user))

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
        build_folder = os.path.join(self.build_folder, "build")
        stage_folder = os.path.join(self.build_folder, "stage")
        self.output.info("-------------- user-config.jam ------------------")
        self.generate_user_config_jam(build_folder)
        self.output.info("-------------- Build libraries ------------------")
        flags = self.get_build_flags(build_folder, stage_folder)
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

    def get_build_flags(self, build_folder, stage_folder):
        flags = ["-a -d2 --debug-configuration --debug-generator --abbreviate-paths --build-type=minimal"]
        flags.append("--build-dir=%s" % build_folder)
        flags.append("--stagedir=%s" % stage_folder)
        flags += self.get_libraries_list()
        flags.append("link=static")
        flags.append("runtime-link=shared")
        flags.append("variant=debug")
        address_model = "64" if self.settings.arch == "x86_64" else "32"
        flags.append("address-model=%s" % address_model)
        if self.options.fPIC:
            flags.append("cxxflags=\"-fPIC\"")
        return flags

    def get_libraries_list(self):
        libs = [
            "--with-iostreams",
            "--with-regex",
            "--with-system"
        ]
        return libs
        
    def generate_user_config_jam(self, build_folder):
        content = ""
        # zlib
        zlib_include = self.deps_cpp_info["zlib"].include_paths[0].replace("\\", "/")
        zlib_libpath = self.deps_cpp_info["zlib"].lib_paths[0].replace("\\", "/")
        zlib_lib = self.deps_cpp_info["zlib"].libs[0]
        content += "using zlib : %s : <include>%s <search>%s <name>%s ;\n" % (self._zlib_version, zlib_include, zlib_libpath, zlib_lib)
        # toolset
        compiler, compiler_version, compiler_exe = self.get_toolset()
        compiler_flags = self.get_compiler_flags()
        content += "using %s : %s : %s : %s;\n" % (compiler, compiler_version, compiler_exe, compiler_flags)
        # write file
        self.output.info("Using current user-config.jam:\n%s" % content)
        fname = os.path.join(build_folder, "user-config.jam")
        tools.save(fname, content)
        
    def get_toolset(self):
        compiler_version = str(self.settings.compiler.version)
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            with tools.vcvars(self.settings):
                self.run("cl.exe")
            compiler_version = "14.1" if compiler_version == "15" else "%s.0" % compiler_version
            return "msvc", compiler_version, "cl.exe"
        elif self.settings.os == "Linux" and self.settings.compiler == "gcc":
            return "gcc", compiler_version[0], "g++"
        
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
