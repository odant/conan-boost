from conans import ConanFile, tools
import os


class BoostConan(ConanFile):
    name = "boost"
    version = "1.66.0"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    description = "Boost provides free peer-reviewed portable C++ source libraries"
    url = "https://github.com/odant/conan-boost"
    settings = "os", "compiler", "build_type", "arch"
    boost_source_archive = "boost_%s.tar.gz" % version.replace(".", "_")
    exports_sources = boost_source_archive, "FindBoost.cmake"
    no_copy_source = True
    build_policy = "missing"

    def source(self):
        tools.unzip(self.boost_source_archive)
        os.remove(self.boost_source_archive)
