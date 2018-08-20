// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/container/pmr/stable_vector.hpp>

#include <string>
#include <iostream>
#include <cstdlib>


int main(int, char**) {

    boost::container::pmr::stable_vector<std::string> v{"First", "Second"};
    const std::string& i = v[1];
    v.erase(v.begin());
    std::cout << "i: " << i << std::endl;
    if (i != "Second") {
        std::cout << "Failed!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}