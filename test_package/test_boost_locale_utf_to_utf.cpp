// Boost::locale (convert utf encoding) test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/locale.hpp>

#include <string>
#include <iostream>
#include <cstdlib>


int main(int, char**) {

    std::locale defaultLocale{boost::locale::generator().generate("")};
    std::locale::global(defaultLocale);
    std::cout.imbue(defaultLocale);
    std::wcout.imbue(defaultLocale);

    {
        std::cout << "-- char(UTF-8) to wchar_t --" << std::endl;

        const std::string in = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in: " << in << " , in.length(): " << in.length() << std::endl;

        const std::wstring normalOut = L"\u042f" L"Z" L"\u0410";
        std::wcout << L"normalOut: " << normalOut << L" , normalOut.length(): " << normalOut.length() << std::endl;

        const std::wstring out = boost::locale::conv::utf_to_utf<wchar_t>(in);

        if (out != normalOut) {

            std::wcout << L"Failed! out not equal normalOut, out: " << out << L" , out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- wchar_t to char(UTF-8) --" << std::endl;

        const std::wstring in = L"\u042f" L"Z" L"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::wcout << L"in: " << in << L" , in.length(): " << in.length() << std::endl;

        const std::string normalOut = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90};
        std::cout << "normalOut: " << normalOut << " , normalOut.length(): " << normalOut.length() << std::endl;

        const std::string out = boost::locale::conv::utf_to_utf<char>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out: " << out << " , out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- char(UTF-8) to char16_t(UTF-16) --" << std::endl;

        const std::string in = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in.length(): " << in.length() << std::endl;

        const std::u16string normalOut = u"\u042f" u"Z" u"\u0410";
        std::cout << "normalOut.length(): " << normalOut.length() << std::endl;

        const std::u16string out = boost::locale::conv::utf_to_utf<char16_t>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- char16_t(UTF-16) to char(UTF-8) --" << std::endl;

        const std::u16string in = u"\u042f" u"Z" u"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in.length(): " << in.length() << std::endl;

        const std::string normalOut = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90};
        std::cout << "normalOut.length(): " << normalOut.length() << std::endl;

        const std::string out = boost::locale::conv::utf_to_utf<char>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- char(UTF-8) to char32_t(UTF-32) --" << std::endl;

        const std::string in = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in.length(): " << in.length() << std::endl;

        const std::u32string normalOut = U"\u042f" U"Z" U"\u0410";
        std::cout << "normalOut.length(): " << normalOut.length() << std::endl;

        const std::u32string out = boost::locale::conv::utf_to_utf<char32_t>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- char32_t(UTF-32) to char(UTF-8) --" << std::endl;

        const std::u32string in = U"\u042f" U"Z" U"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in.length(): " << in.length() << std::endl;

        const std::string normalOut = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90};
        std::cout << "normalOut.length(): " << normalOut.length() << std::endl;

        const std::string out = boost::locale::conv::utf_to_utf<char>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- char16_t(UTF-16) to char32_t(UTF-32) --" << std::endl;

        const std::u16string in = u"\u042f" u"Z" u"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in.length(): " << in.length() << std::endl;

        const std::u32string normalOut = U"\u042f" U"Z" U"\u0410";
        std::cout << "normalOut.length(): " << normalOut.length() << std::endl;

        const std::u32string out = boost::locale::conv::utf_to_utf<char32_t>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- char32_t(UTF-32) to char16_t(UTF-16) --" << std::endl;

        const std::u32string in = U"\u042f" U"Z" U"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        std::cout << "in.length(): " << in.length() << std::endl;

        const std::u16string normalOut = u"\u042f" u"Z" u"\u0410";
        std::cout << "normalOut.length(): " << normalOut.length() << std::endl;

        const std::u16string out = boost::locale::conv::utf_to_utf<char16_t>(in);

        if (out != normalOut) {

            std::cout << "Failed! out not equal normalOut, out.length(): " << out.length() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }

    return EXIT_SUCCESS;
}
