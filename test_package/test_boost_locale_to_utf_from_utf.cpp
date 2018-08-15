// Boost::locale (convert utf encoding) test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/locale.hpp>

#include <string>
#include <iostream>
#include <cstdlib>


int main(int, char**) {

    std::locale defaultLocale{boost::locale::generator().generate("ru_RU.cp1251")};
    std::locale::global(defaultLocale);
    std::cout.imbue(defaultLocale);
    std::wcout.imbue(defaultLocale);

    {
        std::cout << "-- char(cp1251) to wchar_t --" << std::endl;

        const std::string in = {(char)0xdf, 'Z', (char)0xc0}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in cp1251)
        std::cout << "in: " << in << " , in.length(): " << in.length() << std::endl;

        const std::wstring normalOut = L"\u042f" "Z" "\u0410";
        std::wcout << L"normalOut: " << normalOut << L" , normalOut.length(): " << normalOut.length() << std::endl;

        const std::wstring out = boost::locale::conv::to_utf<wchar_t>(in, std::locale{}); // Use global locale
        if (out != normalOut) {
            std::wcout << L"Failed! out not equal normalOut, out: " << out << L" , out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- wchar_t to char(cp1251) --" << std::endl;

        const std::wstring in = L"\u042f" "Z" "\u0410";
        std::wcout << L"in: " << in << L" , in.length(): " << in.length() << std::endl;

        const std::string normalOut = {(char)0xdf, 'Z', (char)0xc0}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in cp1251)
        std::cout << "normalOut: " << normalOut << " , normalOut.length(): " << normalOut.length() << std::endl;

        const std::string out = boost::locale::conv::from_utf(in, std::locale{}); // Use global locale
        if (out != normalOut) {
            std::cout << "Failed! out not equal normalOut, out: " << out << " , out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    return EXIT_SUCCESS;
}
