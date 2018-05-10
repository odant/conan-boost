from conans import ConanFile, tools
from conans.errors import ConanException
import os, glob


def get_safe(options, name):
    try:
        return getattr(options, name, None)
    except ConanException:
        return None


class BoostConan(ConanFile):
    name = "boost"
    version = "1.67.0"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    description = "Boost provides free peer-reviewed portable C++ source libraries"
    url = "https://github.com/odant/conan-boost"
    settings = {
        "os": ["Windows", "Linux"],
        "compiler": ["Visual Studio", "gcc"],
        "build_type": ["Debug", "Release"],
        "arch": ["x86_64", "x86", "mips"]
    }
    #------ internal ------
    _boost_name = "boost_%s" % version.replace(".", "_")
    _boost_archive = _boost_name + ".tar.gz"
    _zlib_version = "[~=1.2.11]"
    #------ internal ------
    exports_sources = _boost_archive, "FindBoost.cmake", "_FindBoost.cmake", "boost.patch"
    no_copy_source = True
    build_policy = "missing"
    short_paths = True

    def configure(self):
        # Only C++11
        if "libcxx" in self.settings.compiler.fields:
            if self.settings.compiler.libcxx == "libstdc++":
                raise Exception("This package is only compatible with libstdc++11")

    def requirements(self):
        self.requires("zlib/%s@%s/stable" % (self._zlib_version, self.user))
        
    def build_requirements(self):
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            self.build_requires("find_sdk_winxp/[~=1.0]@%s/stable" % self.user)

    def source(self):
        self.output.info("-------------- Unzip sources --------------------")
        self.output.info("Current directory => %s" % os.getcwd())
        tools.rmdir(self._boost_name)
        tools.unzip(self._boost_archive)
        os.remove(self._boost_archive)
        tools.patch(patch_file="boost.patch")

    def build(self):
        source_folder = os.path.join(self.source_folder, self._boost_name)
        build_folder = os.path.join(self.build_folder, "build")
        stage_folder = os.path.join(self.build_folder, "stage")
        #
        self.output.info("-------------- Clear cache ----------------------")
        self.clear_cache(source_folder)
        self.output.info("-------------- Bootstrap ------------------------")
        b2 = self.bootstrap(source_folder)
        self.run("%s -v" % b2)
        self.output.info("-------------- user-config.jam ------------------")
        self.generate_user_config_jam(build_folder)
        self.output.info("-------------- Build libraries ------------------")
        flags = self.get_build_flags(build_folder, stage_folder)
        build_env = self.get_build_environment()
        # location user-config.jam
        build_env["BOOST_BUILD_PATH"] = build_folder
        with tools.chdir(source_folder), tools.environment_append(build_env):
            self.output.info("-------------- Environment ----------------------")
            for k, v in sorted(os.environ.items()):
                self.output.info("%s=%s" % (k, v))
                self.output.info("-------------------------------------------------")
            self.output.info("Current directory => %s" % os.getcwd())
            self.run("%s -j%s %s stage" % (b2, tools.cpu_count(), " ".join(flags)))

    def clear_cache(self, source_folder):
        project_conf = os.path.join(source_folder, "project-config.jam")
        if os.path.exists(project_conf):
            os.remove(project_conf)
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            pattern = os.path.join(os.environ["TEMP"], "b2_msvc_*")
            for fpath in glob.glob(pattern):
                self.output.info("Remove %s" % fpath)
                os.remove(fpath)

    def bootstrap(self, source_folder):
        env = self.get_build_environment()
        with tools.chdir(source_folder), tools.environment_append(env):
            cmd = "bootstrap.bat" if tools.os_info.is_windows else "./bootstrap.sh"
            self.output.info("Current directory => %s" % os.getcwd())
            self.run(cmd)
        b2_exe = "b2.exe" if tools.os_info.is_windows else "b2"
        return os.path.join(source_folder, b2_exe)

    def get_build_flags(self, build_folder, stage_folder):
        flags = ["-a -q -d2 --debug-configuration --debug-generator --abbreviate-paths --build-type=minimal"]
        #flags = ["-a -q -d2 --abbreviate-paths --build-type=minimal"]
        flags.append("--build-dir=%s" % build_folder)
        flags.append("--stagedir=%s" % stage_folder)
        flags += self.get_libraries_list()
        toolset, _, _ = self.get_toolset()
        flags.append("toolset=%s" % toolset)
        flags.append("link=static")
        flags.append("runtime-link=shared")
        flags.append("variant=%s" % str(self.settings.build_type).lower())
        address_model = "64" if self.settings.arch == "x86_64" else "32"
        flags.append("address-model=%s" % address_model)
        return flags

    def generate_user_config_jam(self, build_folder):
        content = ""
        # toolset
        compiler, compiler_version, compiler_exe = self.get_toolset()
        compiler_flags = self.get_compiler_flags()
        compiler_options = "<compileflags>\"%s\"" % " ".join(compiler_flags)
        # Disable b2 setup environment for Visual Studio, use manual.
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            fake_env = os.path.join(build_folder, "fake_env.bat").replace("\\", "/")
            tools.save(fake_env, "echo fake_env")
            compiler_options = "%s <setup>\"%s\"" % (compiler_options, fake_env)
        content += "using %s : %s : %s : %s ;\n" % (compiler, compiler_version, compiler_exe, compiler_options)
        # zlib
        zlib_include = self.deps_cpp_info["zlib"].include_paths[0].replace("\\", "/")
        zlib_libpath = self.deps_cpp_info["zlib"].lib_paths[0].replace("\\", "/")
        zlib_lib = self.deps_cpp_info["zlib"].libs[0]
        content += "using zlib : %s : <include>%s <search>%s <name>%s ;\n" % (self._zlib_version, zlib_include, zlib_libpath, zlib_lib)
        # write file
        self.output.info("Using current user-config.jam:\n%s" % content)
        tools.save(os.path.join(build_folder, "user-config.jam"), content)

    def get_build_environment(self):
        env = {}
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            env = tools.vcvars_dict(self.settings, filter_known_paths=True, force=True)
            toolset = str(self.settings.compiler.get_safe("toolset"))
            if toolset.endswith("_xp"):
                import find_sdk_winxp
                env = find_sdk_winxp.dict_append(self.settings.arch, env=env)
        return env
                
    def get_libraries_list(self):
        libs = [
            "--with-atomic",
            "--with-chrono",
            "--with-context",
            "--with-coroutine",
            "--with-date_time",
            "--with-exception",
            "--with-fiber",
            "--with-filesystem",
            "--with-graph",
            "--with-graph_parallel",
            "--with-iostreams",
            "--with-locale",
            "--with-log",
            "--with-math",
            "--with-mpi",
            "--with-program_options",
            #"--with-python",
            "--with-random",
            "--with-regex",
            "--with-serialization",
            "--with-signals",
            "--with-stacktrace",
            "--with-system",
            #"--with-test",
            "--with-thread",
            "--with-timer",
            "--with-type_erasure",
            "--with-wave"
        ]
        # No build Boost.Fiber for WinXP
        toolset = str(self.settings.compiler.get_safe("toolset"))
        if toolset.endswith("_xp"):
            libs.remove("--with-fiber")
        return libs

    def get_toolset(self):
        compiler_version = str(self.settings.compiler.version)
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            compiler_version = "14.1" if compiler_version == "15" else "%s.0" % compiler_version
            return "msvc", compiler_version, "cl.exe"
        elif self.settings.os == "Linux" and self.settings.compiler == "gcc":
            return "gcc", compiler_version[0], "g++"
    
    # List compiler flags
    def get_compiler_flags(self):
        flags = []
        if get_safe(self.options, "fPIC"):
            flags.append("-fPIC")
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            flags.append("/DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE")
            flags.append("/D_CRT_SECURE_NO_WARNINGS")
            flags.append("/D_CRT_NONSTDC_NO_DEPRECATE")
            toolset = str(self.settings.compiler.get_safe("toolset"))
            if toolset.endswith("_xp"):
                _win32_winnt = "0x0502" if self.settings.arch == "x86_64" else "0x0501"
                flags.append("/D_WIN32_WINNT=%s" % _win32_winnt)
        return flags
        
    def package(self):
        self.copy("FindBoost.cmake", dst=".", src=".")
        self.copy("_FindBoost.cmake", dst=".", src=".")
        self.copy(pattern="*", src="%s/boost" % self._boost_name, dst="include/boost")
        self.copy("*.lib", src="stage", dst="lib", keep_path=False)
        self.copy("*.a", src="stage", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.defines.append("BOOST_USE_STATIC_LIBS")
        self.cpp_info.defines.append("BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE")
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            # DISABLES AUTO LINKING! NO SMART AND MAGIC DECISIONS THANKS!
            self.cpp_info.defines.append("BOOST_ALL_NO_LIB")
