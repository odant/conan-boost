///////////////////////////////////////////////////////////////
//  Copyright 2021 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/logged_adaptor.hpp>

#include "test_arithmetic.hpp"

int main()
{
   test<boost::multiprecision::number<boost::multiprecision::logged_adaptor<typename boost::multiprecision::mpz_int::backend_type> > >();
   return boost::report_errors();
}
