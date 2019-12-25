// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018

// Add codecvt facets id for char16_t/char32_t (MSVC MD runtime)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#include <corecrt.h>  // define _Mbstatet


namespace std {

    template<class _Elem,
        class _Byte,
        class _Statype>
        class codecvt;


    class locale {
    public:
        class id {
        private:
            size_t _id{0};
        };
    };


    template<>
    class codecvt<char16_t, char, _Mbstatet> {
    public:
        static __declspec(dllexport) locale::id id;
    };

    template<>
    class codecvt<char32_t, char, _Mbstatet> {
    public:
        static __declspec(dllexport) locale::id id;
    };


    __declspec(dllexport) locale::id codecvt<char16_t, char, _Mbstatet>::id;
    __declspec(dllexport) locale::id codecvt<char32_t, char, _Mbstatet>::id;

} // namespace std
