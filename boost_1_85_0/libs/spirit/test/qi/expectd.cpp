/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2016 Frank Hein, maxence business consulting gmbh

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_directive.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_auxiliary.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/statement.hpp>

#include <string>
#include <iostream>
#include "test.hpp"

int
main()
{
    using namespace boost::spirit;
    using namespace boost::spirit::ascii;
    using spirit_test::test;
    using spirit_test::print_info;
    using boost::spirit::qi::expectation_failure;
    

    {
        try
        {
            BOOST_TEST((test("aa", expect[char_ >> char_])));
            BOOST_TEST((test("aaa", expect[char_ >> char_ >> char_('a')])));
            BOOST_TEST((test("xi", expect[char_('x') >> char_('i')])));
            BOOST_TEST((test("xin", expect[char_('x') >> char_('i') >> char_('n')])));
            BOOST_TEST((!test("xi", expect[char_('y')]))); // should throw!
        }
        catch (expectation_failure<char const*> const& x)
        {
            std::cout << "expected: "; print_info(x.what_);
            std::cout << "got: \"" << x.first << '"' << std::endl;

            BOOST_TEST(boost::get<std::string>(x.what_.value) == "y");
            BOOST_TEST(std::string(x.first, x.last) == "xi");
        }
    }

    {
        try
        {
            BOOST_TEST((!test("xi", expect[char_('x') >> char_('o')])));
        }
        catch (expectation_failure<char const*> const& x)
        {
            std::cout << "expected: "; print_info(x.what_);
            std::cout << "got: \"" << x.first << '"' << std::endl;

            BOOST_TEST(std::string(x.first, x.last) == "xi");
            BOOST_TEST(x.what_.tag == "sequence");
        }
    }

    {
        try
        {
            BOOST_TEST((!test(" x i", expect[char_('x') >> char_('o')], space)));
        }
        catch (expectation_failure<char const*> const& x)
        {
            std::cout << "expected: "; print_info(x.what_);
            std::cout << "got: \"" << x.first << '"' << std::endl;

            BOOST_TEST(std::string(x.first, x.last) == " x i");
            BOOST_TEST(x.what_.tag == "sequence");
        }
    }

    {
        try
        {
            BOOST_TEST((test(" a a", expect[char_ >> char_], space)));
            BOOST_TEST((test(" x i", expect[char_('x') >> char_('i')], space)));
            BOOST_TEST((!test(" x i", expect[char_('y')], space)));
        }
        catch (expectation_failure<char const*> const& x)
        {
            std::cout << "expected: "; print_info(x.what_);
            std::cout << "got: \"" << x.first << '"' << std::endl;

            BOOST_TEST(boost::get<std::string>(x.what_.value) == "y");
            BOOST_TEST(std::string(x.first, x.last) == "x i");
        }
    }

    {
        try
        {
            BOOST_TEST((test("aA", expect[no_case[char_('a') >> 'a']])));
            BOOST_TEST((test("BEGIN END", expect[no_case[lit("begin") >> "end"]], space)));
            BOOST_TEST((!test("BEGIN END", expect[no_case[lit("begin") >> "nend"]], space)));
        }
        catch (expectation_failure<char const*> const& x)
        {
            std::cout << "expected: "; print_info(x.what_);
            std::cout << "got: \"" << x.first << '"' << std::endl;

            BOOST_TEST(x.what_.tag == "sequence");
            BOOST_TEST(std::string(x.first, x.last) == "BEGIN END");
        }
    }

    {
        using boost::spirit::qi::rule;
        using boost::spirit::eps;
        rule<const wchar_t*, void(int)> r;
        r = expect[eps(_r1)];
    }

    return boost::report_errors();
}

