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
typedef char *foreach_container_type;
typedef char const *foreach_const_container_type;
typedef char foreach_value_type;
typedef char &foreach_reference_type;
typedef char const &foreach_const_reference_type;

#include "./utility.hpp"

///////////////////////////////////////////////////////////////////////////////
// define some containers
//
char my_ntcs_buffer[] = "\1\2\3\4\5";
char *my_ntcs  = my_ntcs_buffer;
char const *my_const_ntcs  = my_ntcs;

///////////////////////////////////////////////////////////////////////////////
// test_main
//   
int main()
{
    // non-const containers by reference
    BOOST_TEST(sequence_equal_byref_n_r(my_ntcs, "\5\4\3\2\1"));

    // const containers by reference
    BOOST_TEST(sequence_equal_byref_c_r(my_const_ntcs, "\5\4\3\2\1"));

    // mutate the mutable collections
    mutate_foreach_byref_r(my_ntcs);

    // compare the mutated collections to the actual results
    BOOST_TEST(sequence_equal_byref_n_r(my_ntcs, "\6\5\4\3\2"));

    return boost::report_errors();
}
