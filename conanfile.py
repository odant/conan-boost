# Boost Conan package
# Dmitriy Vetutnev, ODANT, 2018-2020
# Arkady Yudintsev, ODANT, 2021-2025

from conan import ConanFile, tools
from collections import namedtuple
import os, platform, re


class BoostConan(ConanFile):
    name = "boost"
    version = "1.88.0+1"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    description = "Boost provides free peer-reviewed portable C++ source libraries"
    url = "https://github.com/odant/conan-boost"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "with_unit_tests": [True, False],
        "with_icu": [True, False],
        "sp_debug_hooks": [True, False]
    }
    default_options = {
        "with_unit_tests": False,
        "with_icu": True,
        "sp_debug_hooks": False
    }
    #
    _boost_name = "boost_%s" % version.replace(".", "_").split("+", 1)[0].split("-", 1)[0]
    #
    exports_sources = (
        _boost_name + "/*",
        "multiprecision.patch",
        "add_boost_log_codecvt_enable_param.patch",
        "fix_leak_child_process.patch",
        "sp_debug_hooks.patch",
        "add_weak_ptr_operator_equal.patch",
        "shared_mutex_state_64b.patch",
        "fix_std_category_wrapper.patch",
        "fix_filesystem_path_constructor.patch",
        "revert_atomic_v1.88.patch",
        "revert_log_v1.88.patch"
    )
    #
    no_copy_source = True
    build_policy = "missing"
    package_type = "static-library"

    def requirements(self):
        self.requires("zlib-ng/[>=2.2.4]@%s/stable" % self.user)
        self.requires("zstd/[>=1.5.7]@%s/stable" % self.user)
        if self.options.with_icu:
            self.requires("icu/[>=68.2]@%s/stable" % self.user)

    def source(self):
        tools.files.patch(self, patch_file="multiprecision.patch")
        tools.files.patch(self, patch_file="add_boost_log_codecvt_enable_param.patch")
        tools.files.patch(self, patch_file="fix_leak_child_process.patch")
        tools.files.patch(self, patch_file="sp_debug_hooks.patch")
        tools.files.patch(self, patch_file="add_weak_ptr_operator_equal.patch")
        tools.files.patch(self, patch_file="shared_mutex_state_64b.patch")
        tools.files.patch(self, patch_file="fix_std_category_wrapper.patch")
        tools.files.patch(self, patch_file="fix_filesystem_path_constructor.patch")
        if platform.system() != "Windows":
            self.run("chmod a+x %s" % os.path.join(self.source_folder, self._boost_name, "bootstrap.sh"))
            self.run("chmod a+x %s" % os.path.join(self.source_folder, self._boost_name, "tools/build/src/engine/build.sh"))
        else:
            tools.files.patch(self, patch_file="revert_atomic_v1.88.patch")
            tools.files.patch(self, patch_file="revert_log_v1.88.patch")
    
    def generate(self):
        benv = tools.env.VirtualBuildEnv(self)
        benv.generate()
        renv = tools.env.VirtualRunEnv(self)
        renv.generate()
        if tools.microsoft.is_msvc(self):
            vc = tools.microsoft.VCVars(self)
            vc.generate()         

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
        # location user-config.jam
        env = tools.env.Environment()
        env.define("BOOST_BUILD_PATH", build_folder)
        with tools.files.chdir(self, source_folder), env.vars(self).apply():
            self.output.info("-------------- Environment ----------------------")
            for k, v in sorted(os.environ.items()):
                self.output.info("%s=%s" % (k, v))
                self.output.info("-------------------------------------------------")
            self.output.info("Current directory => %s" % os.getcwd())
            build_command = "%s -j%s %s stage" % (b2, tools.build.build_jobs(self), " ".join(flags));
            self.output.info("Build command:\n%s" % build_command)
            self.run(build_command)
        #
        if self.options.with_unit_tests:
            exclude = [
                "disjoint_sets"
            ]
            self.output.info("-------------- Runnig tests ---------------------")
            with tools.files.chdir(self, os.path.join(source_folder, "status")), env.vars(self).apply():
                _exclude =  ','.join(exclude)
                self.run("%s -j%s -q --check-libs-only, --exclude-tests=%s" % (b2, tools.cpu_count(), _exclude))

    def bootstrap(self, source_folder):
        with tools.files.chdir(self, source_folder):
            cmd = "bootstrap.bat" if self.settings.os == "Windows" else "./bootstrap.sh"
            self.output.info("Current directory => %s" % os.getcwd())
            self.run(cmd)
        b2_exe = "b2.exe" if self.settings.os == "Windows" else "b2"
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
        runtime_link = "shared"
        if self.settings.os == "Windows" and (self.settings.compiler == "msvc" or (self.settings.compiler == "clang" and self.settings.compiler.get_safe("runtime_version"))):
            if self.settings.compiler.runtime == "static":
                runtime_link = "static"
        flags.extend([
            "toolset=%s" % toolset,
            "link=static",
            "runtime-link=%s" % runtime_link,
            "variant=%s" % str("Release" if self.settings.build_type == "RelWithDebInfo" else self.settings.build_type).lower(),
            "address-model=%s" % {"x86": "32", "x86_64": "64", "mips": "32", "armv7": "32"}.get(str(self.settings.arch))
        ])
        if self.settings.os == "Windows": 
            flags.append("define=BOOST_USE_WINAPI_VERSION=0x0601")
        # locale use ICU
        if self.options.with_icu:
            # add BOOST_LOG_CXX11_CODECVT_FACETS_FORCE_ENABLE
            flags.append("define=BOOST_LOG_CXX11_CODECVT_FACETS_FORCE_ENABLE")
            icu_path = self.dependencies["icu"].package_folder.replace("\\", "/")
            flags.extend([
                "boost.locale.icu=on",
                "boost.locale.iconv=off",
                "boost.locale.winapi=off",
                "boost.locale.std=off",
                "boost.locale.posix=off",
                "-sICU_PATH=%s" % icu_path
            ])
            for d in self.dependencies["icu"].cpp_info.defines:
                flags.append("define=%s" % d)
        return flags

    def generate_user_config_jam(self, build_folder):
        content = ""
        # toolset
        compiler, compiler_version, compiler_exe = self.get_toolset()
        compiler_flags = self.get_compiler_flags()
        compiler_options = "<compileflags>\"%s\"" % " ".join(compiler_flags)
        # Disable b2 setup environment for Visual Studio, use manual.
        if self.settings.os == "Windows" and self.settings.compiler == "msvc":
            fake_env = os.path.join(build_folder, "fake_env.bat").replace("\\", "/")
            tools.files.save(self, fake_env, "echo fake_env")
            compiler_options = "%s <setup>\"%s\"" % (compiler_options, fake_env)
            content += "using %s : : %s : %s ;\n" % (compiler, compiler_exe, compiler_options)
        else:    
            content += "using %s : %s : %s : %s ;\n" % (compiler, compiler_version, compiler_exe, compiler_options)
        # zlib
        zlib_version = str(self.dependencies["zlib-ng"].ref.version).split("+")[0]
        zlib_cpp_info = self.dependencies["zlib-ng"].cpp_info.components["zlib-ng"] if "zlib-ng" in self.dependencies["zlib-ng"].cpp_info.components.keys() else self.dependencies["zlib-ng"].cpp_info
        zlib_include = zlib_cpp_info.includedirs[0].replace("\\", "/")
        zlib_libpath = zlib_cpp_info.libdirs[0].replace("\\", "/")
        zlib_lib = zlib_cpp_info.libs[0]
        content += "using zlib : %s : <include>%s <search>%s <name>%s ;\n" % (zlib_version, zlib_include, zlib_libpath, zlib_lib)
        #zstd
        zstd_version = str(self.dependencies["zstd"].ref.version).split("+")[0]
        zstd_cpp_info = self.dependencies["zstd"].cpp_info.components["zstdlib"] if "zstdlib" in self.dependencies["zstd"].cpp_info.components.keys() else self.dependencies["zstd"].cpp_info
        zstd_include = zstd_cpp_info.includedirs[0].replace("\\", "/")
        zstd_libpath = zstd_cpp_info.libdirs[0].replace("\\", "/")
        zstd_lib = zstd_cpp_info.libs[0]
        content += "using zstd : %s : <include>%s <search>%s <name>%s ;\n" % (zstd_version, zstd_include, zstd_libpath, zstd_lib)
        # write file
        self.output.info("Using current user-config.jam:\n%s" % content)
        tools.files.save(self, os.path.join(build_folder, "user-config.jam"), content)

    def get_libraries_list(self):
        libs = [
            "--without-python"
        ]
        return libs

    def get_toolset(self):
        compiler_version = str(self.settings.compiler.version)
        if self.settings.os == "Windows":
            if self.settings.compiler == "msvc":
                vs_toolset = str(self.settings.compiler.toolset).lower()
                self.output.info("Using toolset: %s" % vs_toolset)
                if compiler_version == "15":
                    compiler_version = "14.1"
                elif compiler_version == "16":
                    compiler_version = "14.2"
                elif compiler_version == "17":
                    compiler_version = "14.3"
                else:
                    compiler_version = "%s.0" % compiler_version
                return "msvc", compiler_version, "cl.exe"
            elif self.settings.compiler == "clang" and self.settings.compiler.get_safe("runtime_version"):
                self.output.info("Using Clang-win")
                return "clang-win", compiler_version, "clang-cl.exe"
            else:
               raise Exception("Unsupported compiler on Windows!")

        elif self.settings.os == "Linux" and self.settings.compiler == "gcc":
            return "gcc", compiler_version[0], "g++"

    # List compiler flags
    def get_compiler_flags(self):
        flags = [
            "-DBOOST_NO_AUTO_PTR"
        ]
        if self.settings.os != "Windows":
            flags.append("-fPIC")
        if self.settings.os == "Windows":
            flags.append("/D_WIN32_WINNT=0x0601") # 7 or Server 2008 R2
            flags.append("/DBOOST_SYSTEM_USE_UTF8") # boost::system_category return UTF-8 messages
            if self.settings.compiler == "msvc" or (self.settings.compiler == "clang" and self.settings.compiler.get_safe("runtime_version")):
                flags.append("/DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE")
                flags.append("/D_CRT_SECURE_NO_WARNINGS")
                flags.append("/D_CRT_NONSTDC_NO_DEPRECATE")
        #
        if self.options.with_icu:
            if self.settings.os != "Windows":
                # Enable char16_t and char32_t
                flags.extend([
                    "-DBOOST_LOCALE_ENABLE_CHAR16_T",
                    "-DBOOST_LOCALE_ENABLE_CHAR32_T"
                ])
        #
        if self.options.sp_debug_hooks:
            flags.append("-DBOOST_SP_ENABLE_DEBUG_HOOKS")
        #
        return flags

    def package(self):
        tools.files.copy(self, "*", src=os.path.join(self.source_folder, "%s/boost" % self._boost_name), dst=os.path.join(self.package_folder, "include/boost"), keep_path=True)
        tools.files.copy(self, "*.lib", src=os.path.join(self.build_folder, "stage"), dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        tools.files.copy(self, "*.a", src=os.path.join(self.build_folder, "stage"), dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        tools.files.copy(self, "*.cpp", src=os.path.join(self.source_folder,"%s/libs/smart_ptr/extras/src" % self._boost_name), dst=os.path.join(self.package_folder, "include/boost/smart_ptr/extras/src"))

    def package_id(self):
        self.info.options.with_unit_tests = "any"

    _BoostLib = namedtuple("_BoostLib", ("lib_mask", "dependencies"))
    _BoostLibs = {
        "atomic":                     _BoostLib("(?:lib)?boost_atomic(?:$|[\\.-]+.*)",                     []),
        "charconv":                   _BoostLib("(?:lib)?boost_charconv(?:$|[\\.-]+.*)",                   []),
        "chrono":                     _BoostLib("(?:lib)?boost_chrono(?:$|[\\.-]+.*)",                     ["system"]),
        "cobalt":                     _BoostLib("(?:lib)?boost_cobalt(?:$|[\\.-]+.*)",                     ["container", "context", "system"]),
        "container":                  _BoostLib("(?:lib)?boost_container(?:$|[\\.-]+.*)",                  []),
        "context":                    _BoostLib("(?:lib)?boost_context(?:$|[\\.-]+.*)",                    []),
        "contract":                   _BoostLib("(?:lib)?boost_contract(?:$|[\\.-]+.*)",                   ["exception", "thread"]),
        "coroutine":                  _BoostLib("(?:lib)?boost_coroutine(?:$|[\\.-]+.*)",                  ["context", "exception", "system"]),
        "date_time":                  _BoostLib("(?:lib)?boost_date_time(?:$|[\\.-]+.*)",                  []),
        "exception":                  _BoostLib("(?:lib)?boost_exception(?:$|[\\.-]+.*)",                  []),
        "fiber":                      _BoostLib("(?:lib)?boost_fiber(?:$|[\\.-]+.*)",                      ["context", "filesystem"]),
        "fiber_numa":                 _BoostLib("(?:lib)?boost_fiber_numa(?:$|[\\.-]+.*)",                 ["fiber"]),
        "filesystem":                 _BoostLib("(?:lib)?boost_filesystem(?:$|[\\.-]+.*)",                 ["atomic", "system"]),
        "graph":                      _BoostLib("(?:lib)?boost_graph(?:$|[\\.-]+.*)",                      ["math", "random", "regex", "serialization"]),
        "graph_parallel":             _BoostLib("(?:lib)?boost_graph_parallel(?:$|[\\.-]+.*)",             ["graph", "filesystem", "mpi", "random", "serialization"]),
        "iostreams":                  _BoostLib("(?:lib)?boost_iostreams(?:$|[\\.-]+.*)",                  ["random", "regex"]),
        "json":                       _BoostLib("(?:lib)?boost_json(?:$|[\\.-]+.*)",                       ["container", "system"]),
        "locale":                     _BoostLib("(?:lib)?boost_locale(?:$|[\\.-]+.*)",                     ["thread"]),
        "log":                        _BoostLib("(?:lib)?boost_log(?:$|[\\.-]+.*)",                        ["atomic", "date_time", "exception", "filesystem", "random", "regex", "system", "thread"]),
        "log_setup":                  _BoostLib("(?:lib)?boost_log_setup(?:$|[\\.-]+.*)",                  ["log"]),
        "math":                       _BoostLib(None,                                                      []),
        "math_c99":                   _BoostLib("(?:lib)?boost_math_c99(?:$|[\\.-]+.*)",                   ["math"]),
        "math_c99f":                  _BoostLib("(?:lib)?boost_math_c99f(?:$|[\\.-]+.*)",                  ["math"]),
        "math_c99l":                  _BoostLib("(?:lib)?boost_math_c99l(?:$|[\\.-]+.*)",                  ["math"]),
        "math_tr1":                   _BoostLib("(?:lib)?boost_math_tr1(?:$|[\\.-]+.*)",                   ["math"]),
        "math_tr1f":                  _BoostLib("(?:lib)?boost_math_tr1f(?:$|[\\.-]+.*)",                  ["math"]),
        "math_tr1l":                  _BoostLib("(?:lib)?boost_math_tr1l(?:$|[\\.-]+.*)",                  ["math"]),
        "mpi":                        _BoostLib("(?:lib)?boost_mpi(?:$|[\\.-]+.*)",                        ["graph", "serialization"]),
        "nowide":                     _BoostLib("(?:lib)?boost_nowide(?:$|[\\.-]+.*)",                     ["filesystem"]),
        "prg_exec_monitor":           _BoostLib("(?:lib)?boost_prg_exec_monitor(?:$|[\\.-]+.*)",           ["test"]),
        "process":                    _BoostLib("(?:lib)?boost_process(?:$|[\\.-]+.*)",                    ["filesystem", "system", "context"]),
        "program_options":            _BoostLib("(?:lib)?boost_program_options(?:$|[\\.-]+.*)",            []),
        "random":                     _BoostLib("(?:lib)?boost_random(?:$|[\\.-]+.*)",                     ["system"]),
        "regex":                      _BoostLib("(?:lib)?boost_regex(?:$|[\\.-]+.*)",                      []),
        "serialization":              _BoostLib("(?:lib)?boost_serialization(?:$|[\\.-]+.*)",              []),
        "stacktrace":                 _BoostLib(None,                                                      []),
        "stacktrace_addr2line":       _BoostLib("(?:lib)?boost_stacktrace_addr2line(?:$|[\\.-]+.*)",       ["stacktrace"]),
        "stacktrace_backtrace":       _BoostLib("(?:lib)?boost_stacktrace_backtrace(?:$|[\\.-]+.*)",       ["stacktrace"]),
        "stacktrace_basic":           _BoostLib("(?:lib)?boost_stacktrace_basic(?:$|[\\.-]+.*)",           ["stacktrace"]),
        "stacktrace_from_exception":  _BoostLib("(?:lib)?boost_stacktrace_from_exception(?:$|[\\.-]+.*)",  ["stacktrace"]),
        "stacktrace_noop":            _BoostLib("(?:lib)?boost_stacktrace_noop(?:$|[\\.-]+.*)",            ["stacktrace"]),
        "stacktrace_windbg":          _BoostLib("(?:lib)?boost_stacktrace_windbg(?:$|[\\.-]+.*)",          ["stacktrace"]),
        "stacktrace_windbg_cached":   _BoostLib("(?:lib)?boost_stacktrace_windbg_cached(?:$|[\\.-]+.*)",   ["stacktrace"]),
        "system":                     _BoostLib("(?:lib)?boost_system(?:$|[\\.-]+.*)",                     []),
        "test":                       _BoostLib(None,                                                      ["exception"]),
        "test_exec_monitor":          _BoostLib("(?:lib)?boost_test_exec_monitor(?:$|[\\.-]+.*)",          ["test"]),
        "thread":                     _BoostLib("(?:lib)?boost_thread(?:$|[\\.-]+.*)",                     ["atomic", "chrono", "container", "date_time", "exception", "system"]),
        "timer":                      _BoostLib("(?:lib)?boost_timer(?:$|[\\.-]+.*)",                      []),
        "type_erasure":               _BoostLib("(?:lib)?boost_type_erasure(?:$|[\\.-]+.*)",               ["thread"]),
        "unit_test_framework":        _BoostLib("(?:lib)?boost_unit_test_framework(?:$|[\\.-]+.*)",        ["prg_exec_monitor", "test", "test_exec_monitor"]),
        "url":                        _BoostLib("(?:lib)?boost_url(?:$|[\\.-]+.*)",                        ["system"]),
        "wave":                       _BoostLib("(?:lib)?boost_wave(?:$|[\\.-]+.*)",                       ["filesystem", "serialization"]),
        "wserialization":             _BoostLib("(?:lib)?boost_wserialization(?:$|[\\.-]+.*)",             ["serialization"])
    }

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", "Boost")

        self.cpp_info.components["headers"].libs = []
        self.cpp_info.components["headers"].libdirs = []
        self.cpp_info.components["headers"].set_property("cmake_target_name", "Boost::headers")
        self.cpp_info.components["headers"].defines = ["BOOST_USE_STATIC_LIBS", "BOOST_NO_AUTO_PTR"]
        if self.settings.os == "Windows":
            self.cpp_info.components["headers"].defines.append("BOOST_USE_WINAPI_VERSION=0x0601") # 7 or Server 2008 R2
            self.cpp_info.components["headers"].defines.append("BOOST_SYSTEM_USE_UTF8")           # boost::system_category return UTF-8 messages
            if tools.microsoft.is_msvc(self) or (self.settings.compiler == "clang" and self.settings.compiler.get_safe("runtime_version")):
                self.cpp_info.components["headers"].defines.extend([
                    "BOOST_ALL_NO_LIB",                            # DISABLES AUTO LINKING! NO SMART AND MAGIC DECISIONS THANKS!
                    "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE"
                ])
        if self.options.sp_debug_hooks:
            self.cpp_info.components["headers"].defines.append("BOOST_SP_ENABLE_DEBUG_HOOKS")
        if self.options.with_icu:
            self.cpp_info.components["headers"].defines.append("BOOST_LOG_CXX11_CODECVT_FACETS_FORCE_ENABLE")
            if self.settings.os != "Windows":
                # Enable char16_t and char32_t
                self.cpp_info.components["headers"].defines.extend([
                    "BOOST_LOCALE_ENABLE_CHAR16_T",
                    "BOOST_LOCALE_ENABLE_CHAR32_T"
                ])

        #Set external dependencies
        if self.options.with_icu:
            self._BoostLibs["regex"].dependencies.append("icu::icu")
            self._BoostLibs["locale"].dependencies.append("icu::icu")
        self._BoostLibs["iostreams"].dependencies.append("zlib-ng::zlib-ng")
        self._BoostLibs["iostreams"].dependencies.append("zstd::zstdlib")
        
        all_libs = tools.files.collect_libs(self)
        for libname, desc in self._BoostLibs.items():
            self.cpp_info.components[libname].set_property(f"cmake_target_name", f"Boost::{libname}")
            self.cpp_info.components[libname].requires = desc.dependencies + ["headers"]
            libs = []
            if not desc.lib_mask is None:
                pattern = re.compile(desc.lib_mask)
                libs = [lib for lib in all_libs if pattern.match(lib)]
                if not libs:
                    continue;
            self.cpp_info.components[libname].libs = libs
