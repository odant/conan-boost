///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void basic_tests_9()
{
#ifdef TEST_FLOAT
   time_proc("Bessel Functions (16 digit precision)", "double", test_bessel<double>);
   time_proc("Bessel Functions (16 digit precision)", "real_concept", test_bessel<boost::math::concepts::real_concept>);
   time_proc("Bessel Functions (16 digit precision)", "arithmetic_backend<double>", test_bessel<number<arithmetic_backend<double>, et_on> >);
   time_proc("Bessel Functions (16 digit precision)", "arithmetic_backend<double> - no expression templates", test_bessel<number<arithmetic_backend<double>, et_off> >);

   time_proc("Non-central T (16 digit precision)", "double", test_nct<double>);
   time_proc("Non-central T (16 digit precision)", "real_concept", test_nct<boost::math::concepts::real_concept>);
   time_proc("Non-central T (16 digit precision)", "arithmetic_backend<double>", test_nct<number<arithmetic_backend<double>, et_on> >);
   time_proc("Non-central T (16 digit precision)", "arithmetic_backend<double> - no expression templates", test_nct<number<arithmetic_backend<double>, et_off> >);
#endif
}
