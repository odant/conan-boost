// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/random.hpp>

#include <ctime>
#include <iostream>
#include <cstdlib>


int main(int, char**) {

    const std::time_t now = std::time(nullptr);
    boost::random::mt19937 gen{static_cast<std::uint32_t>(now)};
    std::cout << "boost::random::mt19937: " << gen() << std::endl;

    return EXIT_SUCCESS;
}