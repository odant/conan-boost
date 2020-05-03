// Boost::locale (case conversion) test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/locale.hpp>

#include <iostream>
#include <cstdlib>


int main(int, char**) {

    std::locale defaultLocale{boost::locale::generator().generate("")};
    std::locale::global(defaultLocale);
    std::cout.imbue(defaultLocale);
    std::wcout.imbue(defaultLocale);


    {
        std::cout << "-- to_lower char(UTF-8) --" << std::endl;

        const std::string upper = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::string lower = {(char)0xd1, (char)0x8f, 'z', (char)0xd0, (char)0xb0}; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)
        std::cout << "upper: " << upper << " , lower: " << lower << std::endl;

        const std::string toLower = boost::locale::to_lower(upper);
        std::cout << "toLower: " << toLower << std::endl;

        if (toLower != lower) {

            std::cout << "Faild! toLower not equal lower, toLower: " << toLower << " , expected: " << lower;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- to_upper char(UTF-8) --" << std::endl;

        const std::string upper = {(char)0xd0, (char)0xaf, 'Z', (char)0xd0, (char)0x90}; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::string lower = {(char)0xd1, (char)0x8f, 'z', (char)0xd0, (char)0xb0}; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)
        std::cout << "upper: " << upper << " , lower: " << lower << std::endl;

        const std::string toUpper = boost::locale::to_upper(lower);
        std::cout << "toUpper: " << toUpper << std::endl;

        if (toUpper != upper) {

            std::cout << "Faild! toUpper not equal upper, toUpper: " << toUpper << " , expected: " << upper;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- to_lower wchar_t --" << std::endl;

        const std::wstring upper = L"\u042f" L"Z" L"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::wstring lower = L"\u044f" L"z" L"\u0430"; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)
        std::wcout << L"upper: " << upper << L" , lower: " << lower << std::endl;

        const std::wstring toLower = boost::locale::to_lower(upper);
        std::wcout << L"toLower: " << toLower << std::endl;

        if (toLower != lower) {

            std::wcout << L"Faild! toLower not equal lower, toLower: " << toLower << L" , expected: " << lower;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- to_upper wchar_t --" << std::endl;

        const std::wstring upper = L"\u042f" L"Z" L"\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::wstring lower = L"\u044f" L"z" L"\u0430"; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)
        std::wcout << L"upper: " << upper << L" , lower: " << lower << std::endl;

        const std::wstring toUpper = boost::locale::to_upper(lower);
        std::wcout << L"toUpper: " << toUpper << std::endl;

        if (toUpper != upper) {

            std::wcout << L"Faild! toUpper not equal upper, toUpper: " << toUpper << L" , expected: " << upper;
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
#ifdef BOOST_LOCALE_ENABLE_CHAR16_T
    {
        std::cout << "-- to_lower char16_t(UTF-16) --" << std::endl;

        const std::u16string upper = u"\u042f" "Z" "\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::u16string lower = u"\u044f" "z" "\u0430"; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)

        const std::u16string toLower = boost::locale::to_lower(upper);

        if (toLower != lower) {

            std::cout << "Faild! toLower not equal lower";
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- to_upper char16_t(UTF-16) --" << std::endl;

        const std::u16string upper = u"\u042f" "Z" "\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::u16string lower = u"\u044f" "z" "\u0430"; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)

        const std::u16string toUpper = boost::locale::to_upper(lower);

        if (toUpper != upper) {

            std::cout << "Faild! toUpper not equal upper";
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
#endif
#ifdef BOOST_LOCALE_ENABLE_CHAR32_T
    {
        std::cout << "-- to_lower char32_t(UTF-32) --" << std::endl;

        const std::u32string upper = U"\u042f" "Z" "\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::u32string lower = U"\u044f" "z" "\u0430"; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)

        const std::u32string toLower = boost::locale::to_lower(upper);

        if (toLower != lower) {

            std::cout << "Faild! toLower not equal lower";
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
    {
        std::cout << "-- to_upper char32_t(UTF-32) --" << std::endl;

        const std::u32string upper = U"\u042f" "Z" "\u0410"; // Cyrillic capital letter ya, Z, cyrillic capital letter a (ЯZА in UTF-8)
        const std::u32string lower = U"\u044f" "z" "\u0430"; // Cyrillic small letter ya, z, cyrillic small letter a (яzа in UTF-8)

        const std::u32string toUpper = boost::locale::to_upper(lower);

        if (toUpper != upper) {

            std::cout << "Faild! toUpper not equal upper";
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Ok" << std::endl;
    }
#endif

    return EXIT_SUCCESS;
}
