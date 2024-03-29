/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

#include <string>
#include <cstring>
#include <iostream>
#include "test.hpp"

namespace x3 = boost::spirit::x3;

int got_it = 0;

struct my_rule_class
{
    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result
    on_error(Iterator&, Iterator const& last, Exception const& x, Context const&)
    {
        std::cout
            << "Error! Expecting: "
            << x.which()
            << ", got: \""
            << std::string(x.where(), last)
            << "\""
            << std::endl
            ;
        return x3::error_handler_result::fail;
    }

    template <typename Iterator, typename Attribute, typename Context>
    inline void
    on_success(Iterator const&, Iterator const&, Attribute&, Context const&)
    {
        ++got_it;
    }
};

struct on_success_gets_preskipped_iterator
{
    static bool ok;

    template <typename Iterator, typename Attribute, typename Context>
    void on_success(Iterator before, Iterator& after, Attribute&, Context const&)
    {
        ok = ('b' == *before) && (++before == after);
    }
};
bool on_success_gets_preskipped_iterator::ok = false;

struct on_success_advance_iterator
{
    template <typename Iterator, typename Attribute, typename Context>
    void on_success(Iterator const&, Iterator& after, Attribute&, Context const&)
    {
        ++after;
    }
};
struct on_success_advance_iterator_mutref
{
    template <typename Iterator, typename Attribute, typename Context>
    void on_success(Iterator&, Iterator& after, Attribute&, Context const&)
    {
        ++after;
    }
};
struct on_success_advance_iterator_byval
{
    template <typename Iterator, typename Attribute, typename Context>
    void on_success(Iterator, Iterator& after, Attribute&, Context const&)
    {
        ++after;
    }
};

int
main()
{
    using spirit_test::test_attr;
    using spirit_test::test;

    using namespace boost::spirit::x3::ascii;
    using boost::spirit::x3::rule;
    using boost::spirit::x3::int_;
    using boost::spirit::x3::lit;

    { // show that ra = rb and ra %= rb works as expected
        rule<class a, int> ra;
        rule<class b, int> rb;
        int attr;

        auto ra_def = (ra %= int_);
        BOOST_TEST(test_attr("123", ra_def, attr));
        BOOST_TEST(attr == 123);

        auto rb_def = (rb %= ra_def);
        BOOST_TEST(test_attr("123", rb_def, attr));
        BOOST_TEST(attr == 123);

        auto rb_def2 = (rb = ra_def);
        BOOST_TEST(test_attr("123", rb_def2, attr));
        BOOST_TEST(attr == 123);
    }

    { // show that ra %= rb works as expected with semantic actions
        rule<class a, int> ra;
        rule<class b, int> rb;
        int attr;

        auto f = [](auto&){};
        auto ra_def = (ra %= int_[f]);
        BOOST_TEST(test_attr("123", ra_def, attr));
        BOOST_TEST(attr == 123);

        auto ra_def2 = (rb = (ra %= int_[f]));
        BOOST_TEST(test_attr("123", ra_def2, attr));
        BOOST_TEST(attr == 123);
    }


    { // std::string as container attribute with auto rules

        std::string attr;

        // test deduced auto rule behavior

        auto text = rule<class text_id, std::string>()
            = +(!char_(')') >> !char_('>') >> char_);

        attr.clear();
        BOOST_TEST(test_attr("x", text, attr));
        BOOST_TEST(attr == "x");
    }

    { // error handling

        auto r = rule<my_rule_class, char const*>()
            = '(' > int_ > ',' > int_ > ')';

        BOOST_TEST(test("(123,456)", r));
        BOOST_TEST(!test("(abc,def)", r));
        BOOST_TEST(!test("(123,456]", r));
        BOOST_TEST(!test("(123;456)", r));
        BOOST_TEST(!test("[123,456]", r));

        BOOST_TEST(got_it == 1);
    }

    { // on_success gets pre-skipped iterator
        auto r = rule<on_success_gets_preskipped_iterator, char const*>()
            = lit("b");
        BOOST_TEST(test("a b", 'a' >> r, lit(' ')));
        BOOST_TEST(on_success_gets_preskipped_iterator::ok);
    }

    { // on_success handler mutable 'after' iterator
        auto r1 = rule<on_success_advance_iterator, char const*>()
            = lit("ab");
        BOOST_TEST(test("abc", r1));
        auto r2 = rule<on_success_advance_iterator_mutref, char const*>()
            = lit("ab");
        BOOST_TEST(test("abc", r2));
        auto r3 = rule<on_success_advance_iterator_byval, char const*>()
            = lit("ab");
        BOOST_TEST(test("abc", r3));
    }

    {
        typedef boost::variant<double, int> v_type;
        auto r1 = rule<class r1_id, v_type>()
            = int_;
        v_type v;
        BOOST_TEST(test_attr("1", r1, v) && v.which() == 1 &&
            boost::get<int>(v) == 1);

        typedef boost::optional<int> ov_type;
        auto r2 = rule<class r2_id, ov_type>()
            = int_;
        ov_type ov;
        BOOST_TEST(test_attr("1", r2, ov) && ov && boost::get<int>(ov) == 1);
    }

    // test handling of single element fusion sequences
    {
        using boost::fusion::vector;
        using boost::fusion::at_c;
        auto r = rule<class r_id, vector<int>>()
            = int_;

        vector<int> v(0);
        BOOST_TEST(test_attr("1", r, v) && at_c<0>(v) == 1);
    }

    { // attribute compatibility test
        using boost::spirit::x3::rule;
        using boost::spirit::x3::int_;

        auto const expr = int_;

        long long i;
        BOOST_TEST(test_attr("1", expr, i) && i == 1); // ok

        const rule< class int_rule, int > int_rule( "int_rule" );
        auto const int_rule_def = int_;
        auto const start  = int_rule = int_rule_def;

        long long j;
        BOOST_TEST(test_attr("1", start, j) && j == 1); // error
    }

    return boost::report_errors();
}
