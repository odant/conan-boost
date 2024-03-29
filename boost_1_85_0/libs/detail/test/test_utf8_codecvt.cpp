/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_utf8_codecvt.cpp

// (C) Copyright 2002-4 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm> // std::copy
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>
#include <string>

#include <cstddef> // size_t
#include <cwchar>
#include <boost/config.hpp>
#include <boost/core/no_exceptions_support.hpp>

#if !defined(BOOST_NO_EXCEPTIONS)
#include <exception>
#endif

#define BOOST_UTF8_BEGIN_NAMESPACE namespace boost { namespace detail {
#define BOOST_UTF8_END_NAMESPACE } }
#include <boost/detail/utf8_codecvt_facet.hpp>
#include <boost/detail/utf8_codecvt_facet.ipp>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
    using ::wcslen;
#if !defined(UNDER_CE) && !defined(__PGIC__) 
    using ::w_int;
#endif
} // namespace std
#endif

// Note: copied from boost/iostreams/char_traits.hpp
//
// Dinkumware that comes with QNX Momentics 6.3.0, 4.0.2, incorrectly defines
// the EOF and WEOF macros to not std:: qualify the wint_t type (and so does
// Sun C++ 5.8 + STLport 4). Fix by placing the def in this scope.
// NOTE: Use BOOST_WORKAROUND?
#if (defined(__QNX__) && defined(BOOST_DINKUMWARE_STDLIB))  \
    || defined(__SUNPRO_CC)
    using ::std::wint_t;
#endif

#include <boost/core/lightweight_test.hpp>

// Directory of the test executable
std::string executable_dir;

template<std::size_t s>
struct test_data
{
    static unsigned char utf8_encoding[];
    static wchar_t wchar_encoding[];
};

template<>
unsigned char test_data<2>::utf8_encoding[] = {
    0x01,
    0x7f,
    0xc2, 0x80,
    0xdf, 0xbf,
    0xe0, 0xa0, 0x80,
    0xe7, 0xbf, 0xbf
};

template<>
wchar_t test_data<2>::wchar_encoding[] = {
    0x0001,
    0x007f,
    0x0080,
    0x07ff,
    0x0800,
    0x7fff
};

template<>
unsigned char test_data<4>::utf8_encoding[] = {
    0x01,
    0x7f,
    0xc2, 0x80,
    0xdf, 0xbf,
    0xe0, 0xa0, 0x80,
    0xef, 0xbf, 0xbf,
    0xf0, 0x90, 0x80, 0x80,
    0xf4, 0x8f, 0xbf, 0xbf,
    /* codecvt implementations for clang and gcc don't handle more than 21 bits and
     * return eof accordlingly.  So don't test the whole 32 range
     */
    /*
    0xf7, 0xbf, 0xbf, 0xbf,
    0xf8, 0x88, 0x80, 0x80, 0x80,
    0xfb, 0xbf, 0xbf, 0xbf, 0xbf,
    0xfc, 0x84, 0x80, 0x80, 0x80, 0x80,
    0xfd, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf
    */
};

template<>
wchar_t test_data<4>::wchar_encoding[] = {
    (wchar_t)0x00000001,
    (wchar_t)0x0000007f,
    (wchar_t)0x00000080,
    (wchar_t)0x000007ff,
    (wchar_t)0x00000800,
    (wchar_t)0x0000ffff,
    (wchar_t)0x00010000,
    (wchar_t)0x0010ffff,
    /* codecvt implementations for clang and gcc don't handle more than 21 bits and
     * return eof accordlingly.  So don't test the whole 32 range
     */
    /*
    (wchar_t)0x001fffff,
    (wchar_t)0x00200000,
    (wchar_t)0x03ffffff,
    (wchar_t)0x04000000,
    (wchar_t)0x7fffffff
    */
};

int
test_main(int /* argc */, char * /* argv */[]) {
    std::locale utf8_locale
        = std::locale(
            std::locale::classic(),
            new boost::detail::utf8_codecvt_facet
        );

    typedef char utf8_t;
    // define test data compatible with the wchar_t implementation
    // as either ucs-2 or ucs-4 depending on the compiler/library.
    typedef test_data<sizeof(wchar_t)> td;

    // Send our test UTF-8 data to file
    {
        std::ofstream ofs;
        ofs.open((executable_dir + "test.dat").c_str());
        std::copy(
            td::utf8_encoding,
            td::utf8_encoding + sizeof(td::utf8_encoding) / sizeof(unsigned char),
            std::ostream_iterator<utf8_t>(ofs)
        );
    }

    // Read the test data back in, converting to UCS-4 on the way in
    std::vector<wchar_t> from_file;
    {
        std::wifstream ifs;
        ifs.imbue(utf8_locale);
        ifs.open((executable_dir + "test.dat").c_str());

        std::wint_t item = 0;
        // note can't use normal vector from iterator constructor because
        // dinkumware doesn't have it.
        for(;;){
            item = ifs.get();
            if(item == WEOF)
                break;
            //ifs >> item;
            //if(ifs.eof())
            //    break;
            from_file.push_back(item);
        }
    }

    BOOST_TEST(std::equal(from_file.begin(), from_file.end(), td::wchar_encoding));
  
    // Send the UCS4_data back out, converting to UTF-8
    {
        std::wofstream ofs;
        ofs.imbue(utf8_locale);
        ofs.open((executable_dir + "test2.dat").c_str());
        std::copy(
            from_file.begin(),
            from_file.end(),
            std::ostream_iterator<wchar_t, wchar_t>(ofs)
        );
    }

    // Make sure that both files are the same
    {
        typedef std::istream_iterator<utf8_t> is_iter;
        is_iter end_iter;

        std::ifstream ifs1((executable_dir + "test.dat").c_str());
        is_iter it1(ifs1);
        std::vector<utf8_t> data1;
        std::copy(it1, end_iter, std::back_inserter(data1));

        std::ifstream ifs2((executable_dir + "test2.dat").c_str());
        is_iter it2(ifs2);
        std::vector<utf8_t> data2;
        std::copy(it2, end_iter, std::back_inserter(data2));

        BOOST_TEST_ALL_EQ(data1.begin(), data1.end(), data2.begin(), data2.end());
    }

    // some libraries have trouble that only shows up with longer strings

    const wchar_t * test3_data = L"\
    <?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
    <!DOCTYPE boost_serialization>\
    <boost_serialization signature=\"serialization::archive\" version=\"3\">\
    <a class_id=\"0\" tracking_level=\"0\">\
        <b>1</b>\
        <f>96953204</f>\
        <g>177129195</g>\
        <l>1</l>\
        <m>5627</m>\
        <n>23010</n>\
        <o>7419</o>\
        <p>16212</p>\
        <q>4086</q>\
        <r>2749</r>\
        <c>-33</c>\
        <s>124</s>\
        <t>28</t>\
        <u>32225</u>\
        <v>17543</v>\
        <w>0.84431422</w>\
        <x>1.0170664757130923</x>\
        <y>tjbx</y>\
        <z>cuwjentqpkejp</z>\
    </a>\
    </boost_serialization>\
    ";

    // Send the UCS4_data back out, converting to UTF-8
    std::size_t l = std::wcslen(test3_data);
    {
        std::wofstream ofs;
        ofs.imbue(utf8_locale);
        ofs.open((executable_dir + "test3.dat").c_str());
        std::copy(
            test3_data,
            test3_data + l,
            std::ostream_iterator<wchar_t, wchar_t>(ofs)
        );
    }

    // Make sure that both files are the same
    {
        std::wifstream ifs;
        ifs.imbue(utf8_locale);
        ifs.open((executable_dir + "test3.dat").c_str());
        ifs >> std::noskipws;
        BOOST_TEST(
            std::equal(
                test3_data,
                test3_data + l,
                std::istream_iterator<wchar_t, wchar_t>(ifs)
            )
        );
    }

    // Test length calculation
    {
        std::codecvt<wchar_t, char, std::mbstate_t> const& fac = std::use_facet< std::codecvt<wchar_t, char, std::mbstate_t> >(utf8_locale);
        std::mbstate_t mbs = std::mbstate_t();
        const int utf8_len = sizeof(td::utf8_encoding) / sizeof(*td::utf8_encoding);
        int res = fac.length(mbs, reinterpret_cast< const char* >(td::utf8_encoding), reinterpret_cast< const char* >(td::utf8_encoding + utf8_len), ~static_cast< std::size_t >(0u));
        BOOST_TEST_EQ(utf8_len, res);
    }

    // Test that length calculation detects character boundaries
    {
        std::codecvt<wchar_t, char, std::mbstate_t> const& fac = std::use_facet< std::codecvt<wchar_t, char, std::mbstate_t> >(utf8_locale);
        std::mbstate_t mbs = std::mbstate_t();
        // The first 5 bytes of utf8_encoding contain 3 complete UTF-8 characters (taking 4 bytes in total) and 1 byte of an incomplete character.
        // This last byte should not be accounted by length().
        const int input_len = 5;
        const int utf8_len = 4;
        int res = fac.length(mbs, reinterpret_cast< const char* >(td::utf8_encoding), reinterpret_cast< const char* >(td::utf8_encoding + input_len), ~static_cast< std::size_t >(0u));
        BOOST_TEST_EQ(utf8_len, res);
    }

    return EXIT_SUCCESS;
}

int
main(int argc, char * argv[]) {
    if (argc > 0) {
        // We need to save the path to executable to create test files in the same directory.
        // This allows running different configurations of the test (release/debug, different C++ versions, etc.) in parallel.
        std::string exec_name = argv[0];
        std::string::size_type last_dir_sep_pos = exec_name.find_last_of("/\\");
        if (last_dir_sep_pos != std::string::npos)
            executable_dir = exec_name.substr(0, last_dir_sep_pos + 1); // include the trailing directory separator
    }

    int retval = 1;
    BOOST_TRY{
        retval = test_main(argc, argv);
    }
#ifndef BOOST_NO_EXCEPTION_STD_NAMESPACE
    BOOST_CATCH(const std::exception & e){
        BOOST_ERROR(e.what());
    }
#endif
    BOOST_CATCH(...){
        BOOST_ERROR("failed with uncaught exception:");
    }
    BOOST_CATCH_END

    int error_count = boost::report_errors();
    if (error_count > 0)
        retval = error_count;
    return retval;
}
