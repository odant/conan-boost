/*
Copyright 2018 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License,
Version 1.0. (See accompanying file LICENSE_1_0.txt
or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
#ifdef TEST_STD
#include <type_traits>
#else
#include <boost/type_traits/is_unbounded_array.hpp>
#endif
#include "check_integral_constant.hpp"

TT_TEST_BEGIN(is_unbounded_array)

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<void>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int*>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int&>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int[2]>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<const int[3]>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<const volatile int[4]>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int[5][6]>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int[]>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<const int[]>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<const volatile int[]>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int[][7]>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int(&)[8]>::value, false);
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unbounded_array<int(&&)[9]>::value, false);
#endif

TT_TEST_END
