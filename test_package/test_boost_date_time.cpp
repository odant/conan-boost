// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <cstdlib>


int main(int, char**) {

    auto g_date = boost::gregorian::date_from_iso_string("19901015");
    std::cout << "g_date.year(): " << g_date.year() << std::endl;
    std::cout << "g_date.month(): " << g_date.month() << std::endl;
    std::cout << "g_date.day(): " << g_date.day() << std::endl;

    auto pt = boost::posix_time::second_clock::universal_time();
    std::cout << "pt.date(): " << pt.date() << std::endl;
    std::cout << "pt.time_of_day(): " << pt.time_of_day() << std::endl;

    return EXIT_SUCCESS;
}
