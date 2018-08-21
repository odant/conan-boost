// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/system/error_code.hpp>

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


using namespace boost::system;

int main(int, char**) {

    error_code ec_1 = errc::make_error_code(errc::success);
    if (ec_1) {
        std::cout << "Error: errc::make_error_code(errc::success) is true " << __FILE__ << __LINE__ << std::endl;
        return EXIT_FAILURE;
    }

    error_code ec_2 = errc::make_error_code(errc::not_supported);
    if (!ec_2) {
        std::cout << "Error: errc::make_error_code(errc::not_supported) is false " << __FILE__ << __LINE__ << std::endl;
        return EXIT_FAILURE;
    }

#ifdef _WIN32
    std::error_code ec{static_cast<int>(ERROR_FILE_NOT_FOUND), boost::system::system_category()};
    std::cout << "ec.message(): " << ec.message() << std::endl;
#endif

    return EXIT_SUCCESS;
}
