# Boost Conan package
# Dmitriy Vetutnev, ODANT 2018


from conans import ConanFile, tools
from conans.errors import ConanException
import os


def get_safe(options, name):
    try:
        return getattr(options, name, None)
    except ConanException:
        return None


class BoostConan(ConanFile):
    name = "boost"
    version = "1.68.0"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    description = "Boost provides free peer-reviewed portable C++ source libraries"
    url = "https://github.com/odant/conan-boost"
    settings = {
        "os": ["Windows", "Linux"],
        "compiler": ["Visual Studio", "gcc"],
        "build_type": ["Debug", "Release"],
        "arch": ["x86_64", "x86", "mips"]
    }
    options = {
        "fPIC": [True, False]
    }
    default_options = "fPIC=True"
    #
    _boost_name = "boost_%s" % version.replace(".", "_")
    exports_sources = (
        _boost_name + "/*",
        "!" + _boost_name + "/more*", "!*/doc/*", "!*/test/*", # Exclude documentation and tests
        "FindBoost.cmake", "_FindBoost.cmake",
        "multiprecision.patch", "weak_ptr.patch", "system_error_category_english_win.patch"
    )
    #
    no_copy_source = True
    build_policy = "missing"
    short_paths = True
    #
    _zlib_version = "[>=1.2.3]"
    _icu_version = "[>=61.1]"

    def configure(self):
        # Only C++11
        if self.settings.compiler.get_safe("libcxx") == "libstdc++":
            raise ConanException("This package is only compatible with libstdc++11")
        # Position independent code
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("zlib/%s@%s/stable" % (self._zlib_version, self.user))
        self.requires("icu/%s@%s/stable" % (self._icu_version, self.user))

    def build_requirements(self):
        toolset = str(self.settings.compiler.get_safe("toolset"))
        if toolset.endswith("_xp"):
            self.build_requires("find_sdk_winxp/[>=1.0]@%s/stable" % self.user)

    def source(self):
        tools.patch(patch_file="weak_ptr.patch")
        tools.patch(patch_file="multiprecision.patch")
        tools.patch(patch_file="system_error_category_english_win.patch")
        if not tools.os_info.is_windows:
            self.run("chmod a+x %s" % os.path.join(self.source_folder, self._boost_name, "bootstrap.sh"))
            self.run("chmod a+x %s" % os.path.join(self.source_folder, self._boost_name, "tools/build/src/engine/build.sh"))

    def build(self):
        source_folder = os.path.join(self.source_folder, self._boost_name)
        build_folder = os.path.join(self.build_folder, "build")
        stage_folder = os.path.join(self.build_folder, "stage")
        #
        self.output.info("-------------- Bootstrap ------------------------")
        b2 = self.bootstrap(source_folder)
        self.run("%s -v" % b2)
        project_conf = os.path.join(source_folder, "project-config.jam")
        if os.path.exists(project_conf):
            os.remove(project_conf)
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
        flags.extend([
            "--build-dir=%s" % build_folder,
            "--stagedir=%s" % stage_folder
        ])
        flags += self.get_libraries_list()
        toolset, _, _ = self.get_toolset()
        flags.extend([
            "toolset=%s" % toolset,
            "link=static",
            "runtime-link=shared",
            "variant=%s" % str(self.settings.build_type).lower(),
            "address-model=%s" % {"x86": "32", "x86_64": "64", "mips": "32"}.get(str(self.settings.arch))
        ])
        # locale use ICU
        icu_path = self.deps_cpp_info["icu"].rootpath.replace("\\", "/")
        icu_lib_path = self.deps_cpp_info["icu"].lib_paths[0]
        prefix = "" if self.settings.os == "Windows" else "lib"
        ext = ".lib" if self.settings.os == "Windows" else ".so"
        icu_libs = []
        for lib in self.deps_cpp_info["icu"].libs:
            lib = "%s%s%s" % (prefix, lib, ext)
            lib = os.path.join(icu_lib_path, lib).replace("\\", "/")
            icu_libs.append(lib)
        flags.extend([
            "boost.locale.icu=on",
            "boost.locale.iconv=off",
            "boost.locale.winapi=off",
            "boost.locale.std=off",
            "boost.locale.posix=off",
            "-sICU_PATH=%s" % icu_path,
            "-sICU_LINK=\"%s\"" % " ".join(icu_libs)
        ])
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
            "--with-container",
            "--with-context",
            "--with-contract",
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
        flags = [
            "-DBOOST_NO_AUTO_PTR",
            "-DU_DISABLE_RENAMING=1"
        ]
        # Enable char16_t and char32_t
        if self.settings.compiler == "Visual Studio":
            pass
        else:
            flags.extend([
                "-DBOOST_LOCALE_ENABLE_CHAR16_T",
                "-DBOOST_LOCALE_ENABLE_CHAR32_T"
            ])
        if get_safe(self.options, "fPIC"):
            flags.append("-fPIC")
        if self.settings.os == "Windows":
            toolset = str(self.settings.compiler.get_safe("toolset"))
            if toolset.endswith("_xp"):
                _win32_winnt = "0x0502" if self.settings.arch == "x86_64" else "0x0501"
                flags.append("/D_WIN32_WINNT=%s" % _win32_winnt)
            else:
                flags.append("/D_WIN32_WINNT=0x0601") # 7 or Server 2008 R2
            if self.settings.compiler == "Visual Studio":
                flags.append("/DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE")
                flags.append("/D_CRT_SECURE_NO_WARNINGS")
                flags.append("/D_CRT_NONSTDC_NO_DEPRECATE")
        return flags

    def package(self):
        self.copy("FindBoost.cmake", dst=".", src=".")
        self.copy("_FindBoost.cmake", dst=".", src=".")
        self.copy(pattern="*", src="%s/boost" % self._boost_name, dst="include/boost")
        self.copy("*.lib", src="stage", dst="lib", keep_path=False)
        self.copy("*.a", src="stage", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.defines = [
            "BOOST_USE_STATIC_LIBS",
            "BOOST_NO_AUTO_PTR"
        ]
        if self.settings.os == "Windows":
            self.cpp_info.defines.append("_WIN32_WINNT=0x0601") # 7 or Server 2008 R2
        # Enable char16_t and char32_t
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            pass
        else:
            self.cpp_info.defines.extend([
                "BOOST_LOCALE_ENABLE_CHAR16_T",
                "BOOST_LOCALE_ENABLE_CHAR32_T"
            ])
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            self.cpp_info.defines.extend([
                "BOOST_ALL_NO_LIB", # DISABLES AUTO LINKING! NO SMART AND MAGIC DECISIONS THANKS!
                "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE"
            ])
