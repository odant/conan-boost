/*
 [auto_generated]
 libs/numeric/odeint/test/times_time_iterator.cpp

 [begin_description]
 This file tests the times iterator.
 [end_description]

 Copyright 2009-2013 Karsten Ahnert
 Copyright 2009-2013 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#define BOOST_TEST_MODULE odeint_times_time_iterator

#include <iterator>
#include <algorithm>
#include <vector>
#include <iostream>
#include <array>

#include <boost/numeric/odeint/config.hpp>
#include <array>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <boost/numeric/odeint/iterator/times_time_iterator.hpp>
#include "dummy_steppers.hpp"
#include "dummy_odes.hpp"
#include "dummy_observers.hpp"

namespace mpl = boost::mpl;
using namespace boost::numeric::odeint;

typedef dummy_stepper::state_type state_type;
typedef dummy_stepper::value_type value_type;
typedef dummy_stepper::time_type time_type;

BOOST_AUTO_TEST_SUITE( times_time_iterator_test )

typedef mpl::vector<
    dummy_stepper
    , dummy_dense_output_stepper
    > dummy_steppers;

std::array<double,4> times = { 0.0 , 0.1, 0.2, 0.3 };
typedef std::array<double,4>::iterator time_iterator_type;
typedef std::vector< std::pair< state_type , time_type > > result_vector;

BOOST_AUTO_TEST_CASE_TEMPLATE( copy_stepper_iterator , Stepper , dummy_steppers )
{
    typedef times_time_iterator< Stepper , empty_system , state_type , time_iterator_type > iterator_type;
    state_type x = {{ 1.0 }};
    iterator_type iter1 = iterator_type( Stepper() , empty_system() , x , times.begin() , times.end() , 0.1 );
    iterator_type iter2 = iter1;
    BOOST_CHECK_EQUAL( &(iter1->first) , &(iter2->first) );
    BOOST_CHECK_EQUAL( &(iter1->first) , &x );
    BOOST_CHECK( iter1.same( iter2 ) );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( assignment_stepper_iterator , Stepper , dummy_steppers )
{
    std::cout << "assignment" << std::endl;
    typedef times_time_iterator< Stepper , empty_system , state_type , time_iterator_type> iterator_type;
    state_type x1 = {{ 1.0 }} , x2 = {{ 2.0 }};
    iterator_type iter1 = iterator_type( Stepper() , empty_system() , x1 , times.begin() , times.end() , 0.1 );
    iterator_type iter2 = iterator_type( Stepper() , empty_system() , x2 , times.begin() , times.end() , 0.2 );
    BOOST_CHECK_EQUAL( &(iter1->first) , &x1 );
    BOOST_CHECK_EQUAL( &(iter2->first) , &x2 );
    BOOST_CHECK( !iter1.same( iter2 ) );
    iter2 = iter1;
    BOOST_CHECK_EQUAL( &(iter1->first) , &x1 );
    BOOST_CHECK_EQUAL( &(iter2->first) , &x1 );
    BOOST_CHECK( iter1.same( iter2 ) );
}



BOOST_AUTO_TEST_CASE_TEMPLATE( stepper_iterator_factory , Stepper , dummy_steppers )
{
    std::cout << "factory" << std::endl;
    Stepper stepper;
    empty_system system;
    state_type x = {{ 1.0 }};

    std::for_each(
        make_times_time_iterator_begin( stepper , boost::ref( system ) , x , times.begin(), times.end() , 0.1 ) ,
        make_times_time_iterator_end<time_iterator_type>( stepper , boost::ref( system ) , x ) ,
        dummy_observer() );

    // dummy_steppers just add 0.25 at each step, the above for_each leads to 3 do_step calls so x should be 1.75
    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( stepper_range , Stepper , dummy_steppers )
{
    std::cout << "range" << std::endl;
    Stepper stepper;
    empty_system system;
    state_type x = {{ 1.0 }};

    boost::for_each( make_times_time_range( stepper , boost::ref( system ) , x , times.begin() , times.end() , 0.1 ) ,
                     dummy_observer() );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( stepper_iterator_with_reference_wrapper_factory , Stepper , dummy_steppers )
{
    Stepper stepper;
    empty_system system;
    state_type x = {{ 1.0 }};

    std::for_each(
        make_times_time_iterator_begin( boost::ref( stepper ) , boost::ref( system ) , x , times.begin() , times.end() , 0.1 ) ,
        make_times_time_iterator_end<time_iterator_type>( boost::ref( stepper ) , boost::ref( system ) , x ) ,
        dummy_observer() );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( stepper_range_with_reference_wrapper , Stepper , dummy_steppers )
{
    Stepper stepper;
    empty_system system;
    state_type x = {{ 1.0 }};

    boost::for_each( make_times_time_range( boost::ref( stepper ) , boost::ref( system ) , x , times.begin() , times.end(), 0.1 ) ,
                     dummy_observer() );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}

/*
BOOST_AUTO_TEST_CASE_TEMPLATE( transitivity1 , Stepper , dummy_steppers )
{
    typedef times_time_iterator< Stepper , empty_system , state_type , time_iterator_type > stepper_iterator;
    std::cout << "transitivity1" << std::endl;
    state_type x = {{ 1.0 }};
    stepper_iterator first1( Stepper() , empty_system() , x , times.end() , times.end() , 0.1 );
    stepper_iterator last1( Stepper() , empty_system() , x );
    stepper_iterator last2( Stepper() , empty_system() , x );

    BOOST_CHECK( first1 == last1 );
    BOOST_CHECK( first1 == last2 );
    BOOST_CHECK( last1 == last2 );

    first1 = stepper_iterator( Stepper() , empty_system() , x , times.end()-1 , times.end() , 0.1 );
    last1 = stepper_iterator( Stepper() , empty_system() , x );
    BOOST_CHECK( first1 != last1 );
    BOOST_CHECK( ++first1 == last1 );
}
*/

BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm , Stepper , dummy_steppers )
{
    typedef times_time_iterator< Stepper , empty_system , state_type , time_iterator_type> stepper_iterator;
    state_type x = {{ 1.0 }};
    result_vector res;
    stepper_iterator first( Stepper() , empty_system() , x , times.begin() , times.end() , 0.1 );
    stepper_iterator last( Stepper() , empty_system() , x );

    std::copy( first , last , std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );

    for( size_t n=0 ; n<4 ; ++n )
        BOOST_CHECK_CLOSE( res[n].second , times[n] , 1.0e-13 );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );     // the iterator should not iterate over the end
}

BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm_negative_time_step , Stepper , dummy_steppers )
{
    typedef times_time_iterator< Stepper , empty_system , state_type , time_iterator_type > stepper_iterator;
    state_type x = {{ 1.0 }};
    result_vector res;
    std::array<double,4> neg_times = {{ 0.0 , -0.1, -0.2, -0.3 }};
    stepper_iterator first( Stepper() , empty_system() , x , neg_times.begin() , neg_times.end() , -0.1 );
    stepper_iterator last( Stepper() , empty_system() , x );

    std::copy( first , last , std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );

    for( size_t n=0 ; n<4 ; ++n )
        BOOST_CHECK_CLOSE( res[n].second , neg_times[n] , 1.0e-13 );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm_with_factory , Stepper , dummy_steppers )
{
    state_type x = {{ 1.0 }};
    result_vector res;
    std::copy( make_times_time_iterator_begin( Stepper() , empty_system() , x , times.begin() , times.end() , 0.1 ) ,
               make_times_time_iterator_end<time_iterator_type>( Stepper() , empty_system() , x ) ,
               std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );

    for( size_t n=0 ; n<4 ; ++n )
        BOOST_CHECK_CLOSE( res[n].second , times[n] , 1.0e-13 );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm_with_range_factory , Stepper , dummy_steppers )
{
    state_type x = {{ 1.0 }};
    result_vector res;
    boost::range::copy( make_times_time_range( Stepper() , empty_system() , x , times.begin() , times.end() , 0.1 ) ,
                        std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );

    for( size_t n=0 ; n<4 ; ++n )
        BOOST_CHECK_CLOSE( res[n].second , times[n] , 1.0e-13 );

    BOOST_CHECK_CLOSE( x[0] , 1.75 , 1.0e-13 );
}


BOOST_AUTO_TEST_SUITE_END()
