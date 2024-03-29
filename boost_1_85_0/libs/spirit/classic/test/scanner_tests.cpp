/*=============================================================================
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <iostream>
#include <list>


#include <boost/spirit/include/classic_core.hpp>
#include "impl/string_length.hpp"

#include <boost/core/lightweight_test.hpp>

using namespace BOOST_SPIRIT_CLASSIC_NS;

///////////////////////////////////////////////////////////////////////////////
//
//  Scanner tests
//
///////////////////////////////////////////////////////////////////////////////
struct to_upper_iter_policy : public iteration_policy {

    char filter(char ch) const
    { using namespace std; return char(toupper(ch)); }
};

inline bool test_isspace(char c)
{
    using namespace std; return isspace(c) != 0;
}

inline bool test_islower(char c)
{
    using namespace std; return islower(c) != 0;
}

struct skip_white_iter_policy : public iteration_policy {

    template <typename ScannerT>
    void
    advance(ScannerT const& scan) const
    {
        do
            ++scan.first;
        while (!at_end(scan) && test_isspace(get(scan)));
    }
};

void
scanner_tests()
{
    char const* cp = "The Big Brown Fox Jumped \n\tOver The Lazy Dog's Back";
    char const* cp_first = cp;
    char const* cp_last = cp + test_impl::string_length(cp);

    scanner<char const*>
        pp1(cp_first, cp_last);

    //  compile check only...
    scanner<> spp1(pp1); (void)spp1;
    scanner<> spp2(pp1); (void)spp2;
    //    spp1 = spp2;
    //  compile check only...

    while (!pp1.at_end())
    {
        std::cout << *pp1;
        ++pp1;
    }
    std::cout << '\n';
    cp_first = cp;

    std::list<char>              li(cp_first, cp_last);
    std::list<char>::iterator    li_first = li.begin();
    std::list<char>::iterator    li_last = li.end();

    scanner<std::list<char>::iterator>
        pp2(li_first, li_last);

    while (!pp2.at_end())
    {
        std::cout << *pp2;
        ++pp2;
    }
    std::cout << '\n';
    li_first = li.begin();

    scanner<char const*, scanner_policies<to_upper_iter_policy> >
        pp3(cp_first, cp_last);

    while (!pp3.at_end())
    {
        std::cout << *pp3;
        BOOST_TEST(!test_islower(*pp3));
        ++pp3;
    }
    std::cout << '\n';
    cp_first = cp;

    scanner<char const*, scanner_policies<skip_white_iter_policy> >
        pp4(cp_first, cp_last);

    //  compile check only...
    pp1.change_policies(scanner_policies<skip_white_iter_policy>());
    //  compile check only...

    while (!pp4.at_end())
    {
        std::cout << *pp4;
        BOOST_TEST(!test_isspace(*pp4));
        ++pp4;
    }
    std::cout << '\n';
    cp_first = cp;

    std::cout << "sizeof(scanner<>) == " << sizeof(scanner<>) << '\n';

    parse_info<> pi = parse("12abcdefg12345ABCDEFG789", +digit_p, alpha_p);
    BOOST_TEST(pi.hit);
    BOOST_TEST(pi.full);

    pi = parse("abcdefg12345ABCDEFG789", +digit_p, alpha_p);
    BOOST_TEST(pi.hit);
    BOOST_TEST(pi.full);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Main
//
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    scanner_tests();
    return boost::report_errors();
}

