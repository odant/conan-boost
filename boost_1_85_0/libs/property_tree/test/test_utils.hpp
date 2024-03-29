// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_TEST_UTILS_INCLUDED
#define BOOST_PROPERTY_TREE_TEST_UTILS_INCLUDED

#define BOOST_PROPERTY_TREE_DEBUG               // Enable ptree debugging
#include <boost/property_tree/ptree.hpp>

// Do not deprecate insecure CRT calls on VC8
#if (defined(BOOST_MSVC) && (BOOST_MSVC >= 1400) && !defined(_CRT_SECURE_NO_DEPRECATE))
#   define _CRT_SECURE_NO_DEPRECATE
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/property_tree/detail/ptree_utils.hpp>
#include <fstream>
#include <cstring>
#include <sstream>

template<class Ptree>
typename Ptree::size_type total_size(const Ptree &pt)
{
    typename Ptree::size_type size = 1;
    for (typename Ptree::const_iterator it = pt.begin(); it != pt.end(); ++it)
        size += total_size(it->second);
    return size;
}

template<class Ptree>
typename Ptree::size_type total_keys_size(const Ptree &pt)
{
    typename Ptree::size_type size = 0;
    for (typename Ptree::const_iterator it = pt.begin(); it != pt.end(); ++it)
    {
        size += it->first.size();
        size += total_keys_size(it->second);
    }
    return size;
}

template<class Ptree>
typename Ptree::size_type total_data_size(const Ptree &pt)
{
    typename Ptree::size_type size = pt.data().size();
    for (typename Ptree::const_iterator it = pt.begin(); it != pt.end(); ++it)
        size += total_data_size(it->second);
    return size;
}

class test_file
{
public:
    test_file(const char *test_data, const char *filename)
    {
        if (test_data && filename)
        {
            name = filename;
            std::ofstream stream(name.c_str());
            using namespace std;
            stream.write(test_data, strlen(test_data));
            BOOST_TEST(stream.good());
        }
    }
    ~test_file()
    {
        if (!name.empty())
            remove(name.c_str());
    }
private:
    std::string name;
};

template<class Ptree>
Ptree get_test_ptree()
{
    using namespace boost::property_tree;
    typedef typename Ptree::key_type Str;
    Ptree pt;
    pt.put_value(detail::widen<Str>("data0"));
    pt.put(detail::widen<Str>("key1"), detail::widen<Str>("data1"));
    pt.put(detail::widen<Str>("key1.key"), detail::widen<Str>("data2"));
    pt.put(detail::widen<Str>("key2"), detail::widen<Str>("data3"));
    pt.put(detail::widen<Str>("key2.key"), detail::widen<Str>("data4"));
    return pt;
}

// Generic test for file parser
template<class Ptree, class ReadFunc, class WriteFunc>
void generic_parser_test(Ptree &pt,
                         ReadFunc rf, 
                         WriteFunc wf,
                         const char *test_data_1, 
                         const char *test_data_2, 
                         const char *filename_1,
                         const char *filename_2,
                         const char *filename_out)
{

    using namespace boost::property_tree;

    // Create test files
    test_file file_1(test_data_1, filename_1);
    test_file file_2(test_data_2, filename_2);
    test_file file_out("", filename_out);

    rf(filename_1, pt);        // Read file
    wf(filename_out, pt);      // Write file
    Ptree pt2;
    rf(filename_out, pt2);     // Read file again

    // Compare original with read
    BOOST_TEST(pt == pt2);

}

// Generic test for file parser with expected success
template<class Ptree, class ReadFunc, class WriteFunc>
void generic_parser_test_ok(ReadFunc rf, 
                            WriteFunc wf,
                            const char *test_data_1, 
                            const char *test_data_2, 
                            const char *filename_1,
                            const char *filename_2,
                            const char *filename_out,
                            unsigned int total_size, 
                            unsigned int total_data_size, 
                            unsigned int total_keys_size)
{

    using namespace boost::property_tree;

    std::cerr << "(progress) Starting generic parser test with test file \"" << filename_1 << "\"\n";

    // Make sure no instances exist
    //BOOST_CHECK(Ptree::debug_get_instances_count() == 0);

    try
    {

        // Read file
        Ptree pt;
        generic_parser_test<Ptree, ReadFunc, WriteFunc>(pt, rf, wf, 
                                                        test_data_1, test_data_2, 
                                                        filename_1, filename_2, filename_out);

        // Determine total sizes
        typename Ptree::size_type actual_total_size = ::total_size(pt);
        typename Ptree::size_type actual_data_size = ::total_data_size(pt);
        typename Ptree::size_type actual_keys_size = ::total_keys_size(pt);
        if (actual_total_size != total_size ||
            actual_data_size != total_data_size ||
            actual_keys_size != total_keys_size)
            std::cerr << "Sizes: " << (unsigned)::total_size(pt) << ", " << (unsigned)::total_data_size(pt) << ", " << (unsigned)::total_keys_size(pt) << "\n";

        // Check total sizes
        BOOST_TEST(actual_total_size == total_size);
        BOOST_TEST(actual_data_size == total_data_size);
        BOOST_TEST(actual_keys_size == total_keys_size);

    }
    catch (std::runtime_error &e)
    {
        BOOST_ERROR(e.what());
    }

    // Test for leaks
    //BOOST_CHECK(Ptree::debug_get_instances_count() == 0);

}

// Generic test for file parser with expected error
template<class Ptree, class ReadFunc, class WriteFunc, class Error>
void generic_parser_test_error(ReadFunc rf, 
                               WriteFunc wf,
                               const char *test_data_1, 
                               const char *test_data_2, 
                               const char *filename_1,
                               const char *filename_2,
                               const char *filename_out,
                               unsigned long expected_error_line)
{

    std::cerr << "(progress) Starting generic parser test with test file \"" << filename_1 << "\"\n";
    
    // Make sure no instances exist
    //BOOST_CHECK(Ptree::debug_get_instances_count() == 0);

    {

        // Create ptree as a copy of test ptree (to test if read failure does not damage ptree)
        Ptree pt(get_test_ptree<Ptree>());
        try
        {
            generic_parser_test<Ptree, ReadFunc, WriteFunc>(pt, rf, wf,
                                                            test_data_1, test_data_2,
                                                            filename_1, filename_2, filename_out);
            BOOST_ERROR("No required exception thrown");
        }
        catch (Error &e)
        {
            BOOST_TEST(e.line() == expected_error_line);           // Test line number
            BOOST_TEST(pt == get_test_ptree<Ptree>());             // Test if error damaged contents
        }
        catch (...)
        {
            BOOST_ERROR("Invalid exception type thrown");
            throw;
        }

    }

    // Test for leaks
    //BOOST_CHECK(Ptree::debug_get_instances_count() == 0);

}

template <typename Ch> std::basic_ostream<Ch>& errstream();
template <> inline
std::basic_ostream<char>& errstream() { return std::cerr; }
#ifndef BOOST_NO_CWCHAR
template <> inline
std::basic_ostream<wchar_t>& errstream() { return std::wcerr; }
#endif

template <class Ptree, class ReadFunc, class WriteFunc>
void check_exact_roundtrip(ReadFunc rf, WriteFunc wf, const char *test_data) {
    std::cerr << "(progress) Starting exact roundtrip test with test data:\n"
              << test_data << "\n-----\n";
    using namespace boost::property_tree;
    typedef typename Ptree::key_type::value_type Ch;
    typedef typename Ptree::key_type Str;
    Str native_test_data = detail::widen<Str>(test_data);

    std::basic_istringstream<Ch> in_stream(native_test_data);
    std::basic_ostringstream<Ch> out_stream;
    Ptree tree;
    rf(in_stream, tree);
    wf(out_stream, tree);
    std::cerr << "(progress) Roundtripped data:\n";
    errstream<Ch>() << out_stream.str();
    std::cerr << "\n-----\n";
    BOOST_TEST(native_test_data == out_stream.str());
}

#endif
