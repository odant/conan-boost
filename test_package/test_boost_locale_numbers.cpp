// Boost::locale (numbers formatting) test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/locale.hpp>

#include <iostream>
#include <cstdlib>


int main(int, char**) {

    std::locale defaultLocale{boost::locale::generator().generate("")};
    std::locale::global(defaultLocale);
    std::cout.imbue(defaultLocale);
    std::wcout.imbue(defaultLocale);

    std::cout << "boost::locale::as::currency << 42.09378: " << boost::locale::as::currency << 42.09378 << std::endl;
    std::cout << "boost::locale::as::percent << 0.42: " << boost::locale::as::percent << 0.42 << std::endl;
    std::cout << "boost::locale::as::spellout << 42: " << boost::locale::as::spellout << 42 << std::endl;
    std::cout << "boost::locale::as::ordinal << 3: " << boost::locale::as::ordinal << 3 << std::endl;

    return EXIT_SUCCESS;
}
