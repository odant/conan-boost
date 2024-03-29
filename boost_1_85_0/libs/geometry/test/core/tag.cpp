// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2014-2021.
// Modifications copyright (c) 2014-2021 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry_test_common.hpp>

#include <boost/geometry/core/tag.hpp>

#include <boost/geometry/geometries/geometries.hpp>

#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

#include <boost/geometry/geometries/register/linestring.hpp>

#include <vector>
#include <deque>

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)
BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

BOOST_GEOMETRY_REGISTER_LINESTRING_TEMPLATED(std::vector)
BOOST_GEOMETRY_REGISTER_LINESTRING_TEMPLATED(std::deque)


template <typename G, typename Expected>
void test_geometry()
{
    BOOST_CHECK_EQUAL(typeid(typename bg::tag<G>::type).name(),
        typeid(Expected).name());

    static const bool is_same = std::is_same<typename bg::tag<G>::type, Expected>::value;
    BOOST_CHECK(is_same);
}

template <typename P, size_t D>
void test_all()
{
    test_geometry<P, bg::point_tag>();
    test_geometry<P const, bg::point_tag>();
    test_geometry<P*, bg::point_tag>();
    test_geometry<P&, bg::point_tag>();
    test_geometry<P*&, bg::point_tag>();
    test_geometry<const P *, bg::point_tag>();
    test_geometry<bg::model::linestring<P> , bg::linestring_tag>();
    test_geometry<bg::model::linestring<P> *&, bg::linestring_tag>();
    test_geometry<bg::model::ring<P> , bg::ring_tag>();
    test_geometry<bg::model::polygon<P> , bg::polygon_tag>();
    test_geometry<bg::model::box<P> , bg::box_tag>();
    test_geometry<bg::model::segment<P> , bg::segment_tag>();
    test_geometry<bg::model::referring_segment<P const> , bg::segment_tag>();

    test_geometry<std::vector<P>, bg::linestring_tag>();
    test_geometry<std::deque<P>, bg::linestring_tag>();

}

int test_main(int, char* [])
{
    test_geometry<int[2], bg::point_tag>();
    test_geometry<float[2], bg::point_tag>();
    test_geometry<double[2], bg::point_tag>();

    test_geometry<int[3], bg::point_tag>();
    test_geometry<float[3], bg::point_tag>();
    test_geometry<double[3], bg::point_tag>();

    test_geometry<boost::tuple<double, double>, bg::point_tag>();
    test_geometry<boost::tuple<double, double, double>, bg::point_tag>();

    test_all<bg::model::point<int, 2, bg::cs::cartesian>, 2 >();
    test_all<bg::model::point<float, 2, bg::cs::cartesian>, 2 >();
    test_all<bg::model::point<double, 2, bg::cs::cartesian>, 2 >();

    return 0;
}
