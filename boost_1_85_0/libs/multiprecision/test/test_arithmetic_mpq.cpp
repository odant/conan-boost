///////////////////////////////////////////////////////////////
//  Copyright 2012 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#include <boost/multiprecision/gmp.hpp>

#include "test_arithmetic.hpp"

template <>
struct related_type<boost::multiprecision::mpq_rational>
{
   typedef boost::multiprecision::mpz_int type;
};

int main()
{
   test<boost::multiprecision::mpq_rational>();
   return boost::report_errors();
}
