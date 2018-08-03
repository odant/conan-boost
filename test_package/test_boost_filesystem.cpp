// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/filesystem.hpp>

#include <iostream>
#include <cstdlib>


int main(int, char**) {

    const auto currentDir = boost::filesystem::current_path();
    std::cout << "Current directory: " << currentDir.string() << std::endl;

    std::cout << "List directory:" << std::endl;
    boost::filesystem::directory_iterator it{currentDir};
    boost::filesystem::directory_iterator itEnd{};
    for (; it != itEnd; ++it) {
        std::cout << it->path().string() << std::endl;
    }

    return EXIT_SUCCESS;
}