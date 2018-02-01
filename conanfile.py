from conans import ConanFile, tools
import os, shutil


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
    _first_packing = True
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
        self._boost_source_folder = os.path.join(self.source_folder, self._boost_name)
        
    def build(self):
        self.output.info("-------------- Bootstrap ------------------------")
        b2 = self.bootstrap()
        self.run("%s -v" % b2)
        self.output.info("-------------- Build libraries ------------------")
        flags = self.get_build_flags()
        with tools.chdir(self._boost_source_folder):
            self.output.info("Current directory => %s" % os.getcwd())
            self.run("%s %s stage" % (b2, " ".join(flags)))
        
    def bootstrap(self):
        is_windows = tools.os_info.is_windows
        with tools.chdir(self._boost_source_folder):
            cmd = "bootstrap.bat" if is_windows else "./bootstrap.sh"
            self.output.info("Current directory => %s" % os.getcwd())
            self.run(cmd)
        b2_exe = "b2.exe" if is_windows else "b2"
        return os.path.join(self._boost_source_folder, b2_exe)

    def get_build_flags(self):
        flags = ["-a -d2 --debug-configuration --debug-generator --abbreviate-paths --build-type=minimal"]
        self._boost_build_folder = os.path.join(self.build_folder, "build")
        flags.append("--build-dir=%s" % self._boost_build_folder)
        self._boost_stage_folder = os.path.join(self.build_folder, "stage")
        flags.append("--stagedir=%s" % self._boost_stage_folder)
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
        
    def package(self):
        if self._first_packing:
            self.copy("FindBoost.cmake", dst=".", src=".")
            src_headers = os.path.join(self._boost_source_folder, "boost")
            dst_headers = os.path.join(self.package_folder, "include", "boost")
            self.output.info("Moving headers from %s to %s" % (src_headers, dst_headers))
            shutil.move(src_headers, dst_headers)
            self.output.info("Moving headers done")
            self.copy("*.dll", src=self._boost_stage_folder, dst="bin", keep_path=False)
            self.copy("*.lib", src=self._boost_stage_folder, dst="lib", keep_path=False)
            self.copy("*.a", src=self._boost_stage_folder, dst="lib", keep_path=False)
            self.copy("*.so", src=self._boost_stage_folder, dst="lib", keep_path=False)
            self._first_packing = False

    def package_info(self):
        self.cpp_info.defines.append("BOOST_USE_STATIC_LIBS")
        if self.settings.os == "Windows" and self.settings.compiler == "Visual Studio":
            # DISABLES AUTO LINKING! NO SMART AND MAGIC DECISIONS THANKS!
            self.cpp_info.defines.extend(["BOOST_ALL_NO_LIB"])
