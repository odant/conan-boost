// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/system/error_code.hpp>

#include <iostream>
#include <cstdlib>


using namespace boost::system;

int main(int, char**) {

    error_code ec_1 = errc::make_error_code(errc::success);
    if (ec_1) {
        std::cout << "Error: errc::make_error_code(errc::success) is true " << __FILE__ << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    error_code ec_2 = errc::make_error_code(errc::not_supported);
    if (!ec_2) {
        std::cout << "Error: errc::make_error_code(errc::not_supported) is false " << __FILE__ << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
