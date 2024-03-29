
//  (C) Copyright John Maddock 2005. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/is_unsigned.hpp>
#endif
#include "test.hpp"
#include "check_integral_constant.hpp"

#include <climits>

TT_TEST_BEGIN(is_signed)

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<int>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<long>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<short>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<signed char>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned long>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned short>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned char>::value, true);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<UDT>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned&>::value, false);
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned&&>::value, false);
#endif
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned*>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<unsigned[2]>::value, false);

#if defined(BOOST_HAS_INT128) && !defined(TEST_CUDA_DEVICE)
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<boost::int128_type>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<boost::uint128_type>::value, true);
#endif

#if defined(CHAR_MIN)
#if CHAR_MIN != 0
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<char>::value, false);
#else
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<char>::value, true);
#endif
#endif


#if defined(WCHAR_MIN)
#if WCHAR_MIN != 0
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<wchar_t>::value, false);
#else
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_unsigned<wchar_t>::value, true);
#endif
#endif

TT_TEST_END








