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

    def source(self):
        tools.unzip(self._boost_archive)
        os.remove(self._boost_archive)
        
    def build(self):
        b2 = self.bootstrap()
        self.run("%s -v" % b2)
        
    def bootstrap(self):
        boost_source = os.path.join(self.source_folder, self._boost_name)
        is_windows = tools.os_info.is_windows
        with tools.chdir(boost_source):
            cmd = "bootstrap.bat" if is_windows else "./bootstrap.sh"
            self.run(cmd)
        b2_exe = "b2.exe" if is_windows else "b2"
        return os.path.join(boost_source, b2_exe)

