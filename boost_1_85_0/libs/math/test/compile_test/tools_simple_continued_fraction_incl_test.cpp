//  Copyright Matt Borland 2021.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header
// #includes all the files that it needs to.
//
#include <boost/math/tools/simple_continued_fraction.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void compile_and_link_test()
{
    boost::math::tools::simple_continued_fraction<float> f_test(f);
    check_result<float>(f_test.khinchin_geometric_mean());

    boost::math::tools::simple_continued_fraction<double> test(0.0);
    check_result<double>(test.khinchin_geometric_mean());

#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
    boost::math::tools::simple_continued_fraction<long double> ld_test(l);
    check_result<long double>(ld_test.khinchin_geometric_mean());
#endif
}
