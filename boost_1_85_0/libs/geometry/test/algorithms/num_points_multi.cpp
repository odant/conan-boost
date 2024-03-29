// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright (c) 2011-2015 Barend Gehrels, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/num_points.hpp>

#include <boost/geometry/io/wkt/wkt.hpp>

#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/multi_point.hpp>
#include <boost/geometry/geometries/multi_linestring.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>

#include <boost/variant/variant.hpp>


template <typename Geometry>
void check_geometry(Geometry const& geometry, std::string const& wkt, std::size_t expected)
{
    std::size_t detected = bg::num_points(geometry);
    BOOST_CHECK_MESSAGE(detected == expected,
        "num_points: " << wkt
        << " -> Expected: " << expected
        << " detected: " << detected);
}

template <typename Geometry>
void test_geometry(std::string const& wkt, int expected)
{
    Geometry geometry;
    bg::read_wkt(wkt, geometry);
    check_geometry(geometry, wkt, expected);
    check_geometry(boost::variant<Geometry>(geometry), wkt, expected);
}


template <typename Point>
void test_all()
{
    typedef bg::model::polygon<Point> poly;
    typedef bg::model::linestring<Point> ls;
    typedef bg::model::multi_point<Point> mpoint;
    typedef bg::model::multi_linestring<ls> mls;
    typedef bg::model::multi_polygon<poly> mpoly;

    test_geometry<Point>("POINT(0 0)", 1);
    test_geometry<ls>("LINESTRING(0 0,0 1)", 2);
    test_geometry<poly>("POLYGON((0 0,0 1,1 0,0 0))", 4);
    test_geometry<mpoint>("MULTIPOINT((0 0),(0 1),(1 0),(0 0))", 4);
    test_geometry<mls>("MULTILINESTRING((0 0,0 1),(1 0,0 0))", 4);
    test_geometry<mpoly>("MULTIPOLYGON(((0 0,0 1,1 0,0 0)),((10 0,10 1,11 0,10 0)))", 8);
}


int test_main( int , char* [] )
{
    test_all<bg::model::d2::point_xy<double> >();

    return 0;
}
