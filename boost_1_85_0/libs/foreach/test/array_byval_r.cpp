//  (C) Copyright Eric Niebler 2004.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 Revision history:
   25 August 2005 : Initial version.
*/

#include <boost/core/lightweight_test.hpp>
#include <boost/foreach.hpp>

///////////////////////////////////////////////////////////////////////////////
// define the container types, used by utility.hpp to generate the helper functions
typedef int foreach_container_type[5];
typedef int const foreach_const_container_type[5];
typedef int foreach_value_type;
typedef int &foreach_reference_type;
typedef int const &foreach_const_reference_type;

#include "./utility.hpp"

///////////////////////////////////////////////////////////////////////////////
// define some containers
//
int my_array[5] = { 1,2,3,4,5 };
int const (&my_const_array)[5] = my_array;

///////////////////////////////////////////////////////////////////////////////
// test_main
//   
int main()
{
    boost::mpl::false_ *p = BOOST_FOREACH_IS_LIGHTWEIGHT_PROXY(my_array);
    (void)p;

    // non-const containers by value
    BOOST_TEST(sequence_equal_byval_n_r(my_array, "\5\4\3\2\1"));

    // const containers by value
    BOOST_TEST(sequence_equal_byval_c_r(my_const_array, "\5\4\3\2\1"));

    return boost::report_errors();
}
