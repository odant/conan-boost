/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/spirit/include/qi_not_predicate.hpp>

#include <boost/spirit/include/qi_numeric.hpp>

#include <iostream>
#include "test.hpp"

int
main()
{
    using spirit_test::test;
    using boost::spirit::qi::int_;

    {
        BOOST_TEST((!test("1234", !int_)));
        BOOST_TEST((test("abcd", !int_, false)));
        BOOST_TEST((!test("abcd", !!int_, false)));
    }

    return boost::report_errors();
}
