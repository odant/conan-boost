///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void poly_tests_05()
{
#ifdef TEST_MPF
   time_proc("Polynomial Evaluation (50 digit precision)", "mpf_float_50 (no expression templates", test_polynomial<number<gmp_float<50>, et_off> >);
#endif
}
