//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost;
using namespace unit_test;
using namespace accumulators;

///////////////////////////////////////////////////////////////////////////////
// test_stat
//
void test_stat()
{
    accumulator_set<int, stats<tag::moment<2> > > acc1;

    acc1(2); //    4
    acc1(4); //   16
    acc1(5); // + 25
             // = 45 / 3 = 15

    BOOST_CHECK_CLOSE(15., accumulators::moment<2>(acc1), 1e-5);

    accumulator_set<int, stats<tag::moment<5> > > acc2;

    acc2(2); //     32
    acc2(3); //    243
    acc2(4); //   1024
    acc2(5); // + 3125
             // = 4424 / 4 = 1106

    BOOST_CHECK_CLOSE(1106., accumulators::moment<5>(acc2), 1e-5);
}

///////////////////////////////////////////////////////////////////////////////
// test_persistency
//
void test_persistency()
{
    double epsilon = 1e-5;
    std::stringstream ss;
    {
        accumulator_set<int, stats<tag::moment<2> > > acc;
        acc(2); //    4
        acc(4); //   16
        acc(5); // + 25
                 // = 45 / 3 = 15
        BOOST_CHECK_CLOSE(15., accumulators::moment<2>(acc), epsilon);
        boost::archive::text_oarchive oa(ss);
        acc.serialize(oa, 0);
    }
    accumulator_set<int, stats<tag::moment<2> > > acc;
    boost::archive::text_iarchive ia(ss);
    acc.serialize(ia, 0);
    BOOST_CHECK_CLOSE(15., accumulators::moment<2>(acc), epsilon);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("moment test");

    test->add(BOOST_TEST_CASE(&test_stat));
    test->add(BOOST_TEST_CASE(&test_persistency));

    return test;
}

