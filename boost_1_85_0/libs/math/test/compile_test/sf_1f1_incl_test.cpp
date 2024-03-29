//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/special_functions/bessel.hpp>
// #includes all the files that it needs to.
//
#include <boost/math/special_functions/hypergeometric_1F1.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void compile_and_link_test()
{
   check_result<float>(boost::math::hypergeometric_1F1<float>(f, f, f));
   check_result<double>(boost::math::hypergeometric_1F1<double>(d, d, d));
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   check_result<long double>(boost::math::hypergeometric_1F1<long double>(l, l, l));
#endif

   check_result<float>(boost::math::hypergeometric_1F1_regularized<float>(f, f, f));
   check_result<double>(boost::math::hypergeometric_1F1_regularized<double>(d, d, d));
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   check_result<long double>(boost::math::hypergeometric_1F1_regularized<long double>(l, l, l));
#endif

   check_result<float>(boost::math::log_hypergeometric_1F1<float>(f, f, f));
   check_result<double>(boost::math::log_hypergeometric_1F1<double>(d, d, d));
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   check_result<long double>(boost::math::log_hypergeometric_1F1<long double>(l, l, l));
#endif

}
