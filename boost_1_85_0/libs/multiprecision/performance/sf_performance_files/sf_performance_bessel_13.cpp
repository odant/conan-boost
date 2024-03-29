///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void bessel_tests_13()
{
#ifdef TEST_MPFR
   time_proc("Bessel Functions (100 digit precision)", "static_mpfr_float_100", test_bessel<static_mpfr_float_100>);
#endif
}
