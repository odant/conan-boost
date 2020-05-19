/*
 * Test oda boost::gregorian::to_iso_extended_string_type
 * Dmitriy Vetutnev, ODANT 2020
*/


#include <boost/date_time.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <string>
#include <iostream>
#include <cstdlib>


int main(int, char**) {

    const auto time = boost::posix_time::second_clock::local_time();
    const auto date = boost::gregorian::to_iso_extended_string_type<wchar_t>(time.date());

    const std::string result = boost::locale::conv::utf_to_utf<char>(date);
    std::cout << result << std::endl;

    return EXIT_SUCCESS;
}

