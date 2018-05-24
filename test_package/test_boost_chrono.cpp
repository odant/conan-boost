// Boost test for Conan package
// Dmitriy Vetutnev, Odant, 2018


#include <boost/chrono.hpp>
#include <iostream>
#include <cstdlib>


int main(int, char**) {

    std::cout << "boost::chrono::system_clock::now(): " << boost::chrono::system_clock::now() << std::endl;
    std::cout << "boost::chrono::high_resolution_clock::now(): " << boost::chrono::high_resolution_clock::now() << std::endl;

    return EXIT_SUCCESS;
}
