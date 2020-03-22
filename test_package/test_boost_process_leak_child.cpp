// Boost test for Conan package
// Test for leak child process where could not run application (file not exists)
// Dmitriy Vetutnev, ODANT, 2020


#ifndef __unix__
#   error Only for unix-like system.
#endif


#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>


int main(int, char**) {
    const boost::filesystem::path not_exists = boost::filesystem::current_path() / "not_exists";
    if (boost::filesystem::exists(not_exists)) {
        boost::filesystem::remove(not_exists);
    }

    try {
        boost::process::child cp{not_exists};
    }
    catch (const boost::process::process_error& ex) {
        std::cout << "Wthout ec, ex.what(): " << ex.what() << std::endl;
    }
    try {
        std::error_code ec;
        boost::process::child cp{not_exists, ec};
        std::cout << "cp.pid() " << cp.id() << std::endl;
    }
    catch (const boost::process::process_error& ex) {
        std::cout << "With ec, failure! ex.what(): " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    const boost::filesystem::path p = boost::process::search_path("pgrep");
    assert(!p.empty());
    const std::string pid = std::to_string(::getpid());

    boost::process::ipstream out;

    boost::process::child cp{p, "--parent", pid,
                            boost::process::std_out > out};
    cp.join();

    std::vector<std::string> result;
    while (out.good()) {
        std::string s;
        out >> s;
        if (!s.empty()) {
            result.push_back(std::move(s));
        }
    }

    if (result.empty()) {
        std::cout << "Zombie process not found. Ok." << std::endl;
        return EXIT_SUCCESS;
    }
    else {
        std::cout << "Failure! Zombie found:" << std::endl;
        for (const auto& i : result) {
            std::cout << i << std::endl;
        }
        return EXIT_FAILURE;
    }
}
