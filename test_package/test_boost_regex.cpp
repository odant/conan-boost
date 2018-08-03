// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/regex.hpp>

#include <iostream>
#include <string>
#include <cstdlib>


int main(int, char**) {

    std::string s = "Test Boost::regex";
    boost::regex re{R"rr(\w+\s\w+::\w+)rr"};
    std::cout << std::boolalpha << boost::regex_match(s, re) << std::endl;
    if(!boost::regex_match(s, re)) {
        std::cout << "Error: boost::regex_match(s, re) is false " << __FILE__ << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
