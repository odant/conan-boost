// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2013-2015 Adam Wulkiewicz, Lodz, Poland.

// This file was modified by Oracle on 2017-2022.
// Modifications copyright (c) 2017-2022 Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GEOMETRY_TEST_COVERED_BY_HPP
#define BOOST_GEOMETRY_TEST_COVERED_BY_HPP


#include <geometry_test_common.hpp>

#include <boost/variant/variant.hpp>

#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/core/ring_type.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_linestring.hpp>
#include <boost/geometry/io/wkt/read.hpp>
#include <boost/geometry/strategies/strategies.hpp>

template <typename P>
using ls = bg::model::linestring<P>;

template <typename P>
using box = bg::model::box<P>;

template <typename P>
using ring = bg::model::ring<P>;

template <typename P>
using poly = bg::model::polygon<P>;

template <typename P>
using mpt = bg::model::multi_point<P>;

template <typename P>
using mls = bg::model::multi_linestring<ls<P>>;

template <typename P>
using mpoly = bg::model::multi_polygon<poly<P>>;


struct no_strategy {};

template <typename Geometry1, typename Geometry2, typename Strategy>
bool call_covered_by(Geometry1 const& geometry1,
                     Geometry2 const& geometry2,
                     Strategy const& strategy)
{
    return bg::covered_by(geometry1, geometry2, strategy);
}

template <typename Geometry1, typename Geometry2>
bool call_covered_by(Geometry1 const& geometry1,
                     Geometry2 const& geometry2,
                     no_strategy)
{
    return bg::covered_by(geometry1, geometry2);
}

template <typename Geometry1, typename Geometry2, typename Strategy>
void check_geometry(Geometry1 const& geometry1,
                    Geometry2 const& geometry2,
                    std::string const& wkt1,
                    std::string const& wkt2,
                    bool expected,
                    Strategy const& strategy)
{
    bool detected = call_covered_by(geometry1, geometry2, strategy);

    BOOST_CHECK_MESSAGE(detected == expected,
        "covered_by: " << wkt1
        << " in " << wkt2
        << " -> Expected: " << expected
        << " detected: " << detected);
}

template <typename Geometry1, typename Geometry2>
void test_geometry(std::string const& wkt1,
        std::string const& wkt2, bool expected)
{
    Geometry1 geometry1;
    Geometry2 geometry2;
    bg::read_wkt(wkt1, geometry1);
    bg::read_wkt(wkt2, geometry2);
    boost::variant<Geometry1> v1(geometry1);
    boost::variant<Geometry2> v2(geometry2);

    using strategy_type = typename bg::strategies::relate::services::default_strategy
        <
            Geometry1, Geometry2
        >::type;

    check_geometry(geometry1, geometry2, wkt1, wkt2, expected, no_strategy());
    check_geometry(geometry1, geometry2, wkt1, wkt2, expected, strategy_type());
    check_geometry(v1, geometry2, wkt1, wkt2, expected, no_strategy());
    check_geometry(geometry1, v2, wkt1, wkt2, expected, no_strategy());
    check_geometry(v1, v2, wkt1, wkt2, expected, no_strategy());
}

template <typename Geometry1, typename Geometry2, typename Strategy>
void test_geometry(std::string const& wkt1,
                   std::string const& wkt2,
                   bool expected,
                   Strategy const& strategy)
{
    Geometry1 geometry1;
    Geometry2 geometry2;
    bg::read_wkt(wkt1, geometry1);
    bg::read_wkt(wkt2, geometry2);

    check_geometry(geometry1, geometry2, wkt1, wkt2, expected, strategy);
}

/*

template <typename Point, bool Clockwise, bool Closed>
void test_ordered_ring(std::string const& wkt_point,
        std::string const& wkt_geometry, bool expected)
{
    using ring_type = bg::model::ring<Point, Clockwise, Closed>;
    ring_type ring;
    Point point;

    bg::read_wkt(wkt_geometry, ring);
    if (! Clockwise)
    {
        std::reverse(boost::begin(ring), boost::end(ring));
    }
    if (! Closed)
    {
        ring.resize(ring.size() - 1);
    }

    bg::read_wkt(wkt_point, point);

    bool detected = bg::covered_by(point, ring);

    BOOST_CHECK_MESSAGE(detected == expected,
        "covered_by: " << wkt_point
        << " in " << wkt_geometry
        << " -> Expected: " << expected
        << " detected: " << detected
        << " clockwise: " << int(Clockwise)
        << " closed: " << int(Closed)
        );

    // other strategy (note that this one cannot detect OnBorder
    // (without modifications)

    bg::strategy::covered_by::franklin<Point> franklin;
    detected = bg::covered_by(point, ring, franklin);
    if (! on_border)
    {
        BOOST_CHECK_MESSAGE(detected == expected,
            "covered_by: " << wkt_point
            << " in " << wkt_geometry
            << " -> Expected: " << expected
            << " detected: " << detected
            << " clockwise: " << int(Clockwise)
            << " closed: " << int(Closed)
            );
    }


    bg::strategy::covered_by::crossings_multiply<Point> cm;
    detected = bg::covered_by(point, ring, cm);
    if (! on_border)
    {
        BOOST_CHECK_MESSAGE(detected == expected,
            "covered_by: " << wkt_point
            << " in " << wkt_geometry
            << " -> Expected: " << expected
            << " detected: " << detected
            << " clockwise: " << int(Clockwise)
            << " closed: " << int(Closed)
            );
    }
}

template <typename Point>
void test_ring(std::string const& wkt_point,
        std::string const& wkt_geometry,
        bool expected)
{
    test_ordered_ring<Point, true, true>(wkt_point, wkt_geometry, expected);
    test_ordered_ring<Point, false, true>(wkt_point, wkt_geometry, expected);
    test_ordered_ring<Point, true, false>(wkt_point, wkt_geometry, expected);
    test_ordered_ring<Point, false, false>(wkt_point, wkt_geometry, expected);
    test_geometry<Point, bg::model::polygon<Point> >(wkt_point, wkt_geometry, expected);
}
*/

#endif
