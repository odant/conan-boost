
//  (C) Copyright John Maddock 2000. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CHECK_TYPE_HPP
#define BOOST_CHECK_TYPE_HPP

#include "test.hpp"
#include <boost/type_traits/is_same.hpp>

/*
macro:
BOOST_CHECK_TYPE(type_expression, expected_type)

type_expression:  an expression that evaluates to a typename.
expected_value:   the type we expect to find.
*/

#ifdef BOOST_BORLANDC
#pragma option -w-8008 -w-8066 -w-8019
#endif

#ifdef TEST_VIA_STATIC_ASSERT

#define BOOST_CHECK_TYPE(type_expression, expected_type) static_assert(::boost::is_same< type_expression, expected_type >::value, BOOST_STRINGIZE(type_expression) " == " BOOST_STRINGIZE(expected_type))

#define BOOST_CHECK_TYPE3(type_expression, type_expression_suffix, expected_type) static_assert(::boost::is_same< type_expression, type_expression_suffix, expected_type >::value, BOOST_STRINGIZE(type_expression) "," BOOST_STRINGIZE(type_expression_suffix) " == " BOOST_STRINGIZE(expected_type))

#define BOOST_CHECK_TYPE4(type_expression, suffix1, suffix2, expected_type) static_assert(::boost::is_same< type_expression, suffix1, suffix2, expected_type >::value, BOOST_STRINGIZE(type_expression) "," BOOST_STRINGIZE(suffix1) "," BOOST_STRINGIZE(suffix2) " == " BOOST_STRINGIZE(expected_type))

#else

#define BOOST_CHECK_TYPE(type_expression, expected_type)\
do{\
   if(!::boost::is_same< type_expression, expected_type >::value){\
   BOOST_CHECK_MESSAGE(false, "The expression: \"" << BOOST_STRINGIZE(expression)\
      << "\" did not have the expected type:\n\tevaluating:   boost::is_same<"\
      << BOOST_STRINGIZE(type_expression) << ", " << BOOST_STRINGIZE(expected_type)\
      << ">" << "\n\tfound:        "\
      << typeid(::boost::is_same< type_expression, expected_type >).name());\
   }else\
      BOOST_CHECK_MESSAGE(true, "Validating Type Expression: \""\
         << BOOST_STRINGIZE(expression) << "\"");\
}while(0)

#define BOOST_CHECK_TYPE3(type_expression, type_expression_suffix, expected_type)\
do{\
   if(!::boost::is_same< type_expression, type_expression_suffix, expected_type >::value){\
   BOOST_CHECK_MESSAGE(false, "The expression: \"" << BOOST_STRINGIZE(expression)\
      << "\" did not have the expected type:\n\tevaluating:   boost::is_same<"\
      << BOOST_STRINGIZE((type_expression, type_expression_suffix)) << ", " << BOOST_STRINGIZE(expected_type)\
      << ">" << "\n\tfound:        "\
      << typeid(::boost::is_same< type_expression, type_expression_suffix, expected_type >).name());\
   }else\
      BOOST_CHECK_MESSAGE(true, "Validating Type Expression: \""\
         << BOOST_STRINGIZE(expression) << "\"");\
}while(0)

#define BOOST_CHECK_TYPE4(type_expression, suffix1, suffix2, expected_type)\
do{\
   if(!::boost::is_same< type_expression, suffix1, suffix2, expected_type >::value){\
   BOOST_CHECK_MESSAGE(false, "The expression: \"" << BOOST_STRINGIZE(expression)\
      << "\" did not have the expected type:\n\tevaluating:   boost::is_same<"\
      << BOOST_STRINGIZE((type_expression, suffix1, suffix2)) << ", " << BOOST_STRINGIZE(expected_type)\
      << ">" << "\n\tfound:        "\
      << typeid(::boost::is_same< type_expression, suffix1, suffix2, expected_type >).name());\
   }else\
      BOOST_CHECK_MESSAGE(true, "Validating Type Expression: \""\
         << BOOST_STRINGIZE(expression) << "\"");\
}while(0)

#endif

#endif
