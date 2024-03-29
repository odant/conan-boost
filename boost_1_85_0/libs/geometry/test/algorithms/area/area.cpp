// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2015-2021.
// Modifications copyright (c) 2015-2021, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <algorithms/area/test_area.hpp>

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/geometry_collection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/adapted/boost_variant.hpp>
#include <boost/geometry/geometries/adapted/boost_variant2.hpp>

#include <boost/geometry/strategy/cartesian/precise_area.hpp>

#include <test_geometries/all_custom_ring.hpp>
#include <test_geometries/all_custom_polygon.hpp>
//#define BOOST_GEOMETRY_TEST_DEBUG

#include <boost/multiprecision/cpp_dec_float.hpp>

template <typename Polygon>
void test_polygon()
{
    // Rotated square, length=sqrt(2) -> area=2
    test_geometry<Polygon>("POLYGON((1 1,2 2,3 1,2 0,1 1))", 2.0);
    test_geometry<Polygon>("POLYGON((1 1,2 2,3 1,2 0,1 1))", 2.0);
    test_geometry<Polygon>("POLYGON((0 0,0 7,4 2,2 0,0 0))", 16.0);
    test_geometry<Polygon>("POLYGON((1 1,2 1,2 2,1 2,1 1))", -1.0);
    test_geometry<Polygon>("POLYGON((0 0,0 7,4 2,2 0,0 0), (1 1,2 1,2 2,1 2,1 1))", 15.0);
}


template <typename P>
void test_all()
{
    test_geometry<bg::model::box<P> >("POLYGON((0 0,2 2))", 4.0);
    test_geometry<bg::model::box<P> >("POLYGON((2 2,0 0))", 4.0);

    test_polygon<bg::model::polygon<P> >();
    test_polygon<all_custom_polygon<P> >();

    // clockwise rings (second is wrongly ordered)
    test_geometry<bg::model::ring<P> >("POLYGON((0 0,0 7,4 2,2 0,0 0))", 16.0);
    test_geometry<bg::model::ring<P> >("POLYGON((0 0,2 0,4 2,0 7,0 0))", -16.0);

    test_geometry<all_custom_ring<P> >("POLYGON((0 0,0 7,4 2,2 0,0 0))", 16.0);

    // ccw
    test_geometry<bg::model::polygon<P, false> >
        ("POLYGON((0 0,0 7,4 2,2 0,0 0), (1 1,2 1,2 2,1 2,1 1))", -15.0);

    test_geometry<bg::model::polygon<P, false> >
        ("POLYGON((1 0,0 1,-1 0,0 -1,1 0))", 2);

    typedef typename bg::coordinate_type<P>::type coord_type;
    if (BOOST_GEOMETRY_CONDITION((std::is_same<coord_type, double>::value)))
    {
        test_geometry<bg::model::polygon<P, false, false> >
            ("POLYGON((100000001 100000000, 100000000 100000001, \
                       99999999 100000000, 100000000  99999999))", 2);
    }
    else if (BOOST_GEOMETRY_CONDITION((std::is_same<coord_type, float>::value)))
    {
        test_geometry<bg::model::polygon<P, false, false> >
            ("POLYGON((100001 100000, 100000 100001, \
                       99999 100000, 100000  99999))", 2);
    }
}

template <typename P>
void test_ccw()
{
    typedef typename bg::coordinate_type<P>::type ct;
    bg::model::polygon<P, false> ccw_polygon;
    // counterclockwise rings (second is wrongly ordered)
    std::string poly1 = "POLYGON((1 1,2 2,3 1,2 0,1 1))";
    std::string poly2 = "POLYGON((1 1,2 0,3 1,2 2,1 1))";
    std::string poly3 = "POLYGON((0 0,0 7,4 2,2 0,0 0))";
    std::string poly4 = "POLYGON((0 0,2 0,4 2,0 7,0 0))";

    bg::read_wkt(poly1, ccw_polygon);
    ct area1 = bg::area(ccw_polygon);
    bg::read_wkt(poly2, ccw_polygon);
    ct area2 = bg::area(ccw_polygon);
    bg::read_wkt(poly3, ccw_polygon);
    ct area3 = bg::area(ccw_polygon);
    bg::read_wkt(poly4, ccw_polygon);
    ct area4 = bg::area(ccw_polygon);
    BOOST_CHECK_CLOSE(area1, -1 * area2, 0.001);
    BOOST_CHECK_CLOSE(area3, -1 * area4, 0.001);
}

template <typename P, typename CT>
void test_open(CT expected_area)
{
    typedef bg::model::polygon<P, true, false> open_polygon;
    test_geometry<open_polygon>("POLYGON((1 1,2 2,3 1,2 0))", expected_area);
    // Note the triangular testcase used in CCW is not sensible for open/close
}

template <typename P, typename CT>
void test_open_ccw(CT expected_area)
{
    typedef bg::model::polygon<P, false, false> open_polygon;
    test_geometry<open_polygon>("POLYGON((1 1,2 0,3 1,2 2))", expected_area);
    // Note the triangular testcase used in CCW is not sensible for open/close
}

template <typename P>
void test_poles_ccw()
{
    typedef typename bg::coordinate_type<P>::type ct;
    bg::model::polygon<P, false> polygon;

    std::string poly1 = "POLYGON((45 45,45 95,95 45,45 45))";
    std::string poly2 = "POLYGON((45 45,95 45,45 95,45 45))";
    std::string poly3 = "POLYGON((45 -45,45 -95,95 -45,45 -45))";
    std::string poly4 = "POLYGON((45 -45,95 -45,45 -95,45 -45))";

    bg::read_wkt(poly1, polygon);
    ct area1 = bg::area(polygon);
    bg::read_wkt(poly2, polygon);
    ct area2 = bg::area(polygon);
    bg::read_wkt(poly3, polygon);
    ct area3 = bg::area(polygon);
    bg::read_wkt(poly4, polygon);
    ct area4 = bg::area(polygon);
    BOOST_CHECK_CLOSE(area1, -1 * area2, 0.001);
    BOOST_CHECK_CLOSE(area3, -1 * area4, 0.001);
}

template <typename P>
void test_empty_input()
{
    bg::model::polygon<P> poly_empty;
    bg::model::ring<P> ring_empty;

    test_empty_input(poly_empty);
    test_empty_input(ring_empty);
}

void test_large_integers()
{
    typedef bg::model::point<int, 2, bg::cs::cartesian> int_point_type;
    typedef bg::model::point<double, 2, bg::cs::cartesian> double_point_type;

    bg::model::polygon<int_point_type> int_poly;
    bg::model::polygon<double_point_type> double_poly;

    std::string const polygon_li = "POLYGON((1872000 528000,1872000 192000,\
                                   1536119 192000,1536000 528000,1200000 528000,\
                                   1200000 863880,1536000 863880,1872000 863880,\
                                   1872000 528000))";
    bg::read_wkt(polygon_li, int_poly);
    bg::read_wkt(polygon_li, double_poly);

    double int_area = bg::area(int_poly);
    double double_area = bg::area(double_poly);

    BOOST_CHECK_CLOSE(int_area, double_area, 0.0001);
}

struct precise_cartesian : bg::strategies::detail::cartesian_base
{
    template <typename Geometry>
    static auto area(Geometry const&)
    {
        return bg::strategy::area::precise_cartesian<>();
    }
};

void test_accurate_sum_strategy()
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> point_type;
    typedef bg::model::point
        <
            boost::multiprecision::cpp_dec_float_50,
            2,
            bg::cs::cartesian
        > mp_point_type;

    auto const poly0_string = "POLYGON((0 0,0 1,1 0,0 0))";

    bg::model::polygon<point_type> poly0;
    bg::read_wkt(poly0_string, poly0);

    BOOST_CHECK_CLOSE(bg::area(poly0), 0.5, 0.0001);
    BOOST_CHECK_CLOSE(bg::area(poly0, precise_cartesian()), 0.5, 0.0001);

    bg::model::polygon<mp_point_type> mp_poly0;
    bg::read_wkt(poly0_string, mp_poly0);

    BOOST_CHECK_CLOSE(bg::area(mp_poly0), 0.5, 0.0001);

    auto const poly1_string = "POLYGON((0.10000000000000001 0.10000000000000001,\
            0.20000000000000001 0.20000000000000004,\
            0.79999999999999993 0.80000000000000004,\
            1.267650600228229e30 1.2676506002282291e30,\
            0.10000000000000001 0.10000000000000001))";

    bg::model::polygon<point_type> poly1;
    bg::read_wkt(poly1_string, poly1);

    BOOST_CHECK_CLOSE(bg::area(poly1), 0, 0.0001);
    BOOST_CHECK_CLOSE(bg::area(poly1, precise_cartesian()), -0.315, 0.0001);

    bg::model::polygon<mp_point_type> mp_poly1;
    bg::read_wkt(poly1_string, mp_poly1);

    BOOST_CHECK_CLOSE(bg::area(mp_poly1), 34720783012552.6, 0.0001);

    auto const poly2_string = "POLYGON((1.267650600228229e30 1.2676506002282291e30,\
            0.8 0.8,0.2 0.2,0.1 0.1,1.267650600228229e30 1.2676506002282291e30))";

    bg::model::polygon<point_type> poly2;
    bg::read_wkt(poly2_string, poly2);

    BOOST_CHECK_CLOSE(bg::area(poly2), 0, 0.0001);
    BOOST_CHECK_CLOSE(bg::area(poly2, precise_cartesian()), 0.315, 0.0001);

    bg::model::polygon<mp_point_type> mp_poly2;
    bg::read_wkt(poly2_string, mp_poly2);

    BOOST_CHECK_CLOSE(bg::area(mp_poly2), 35000000000000, 0.0001);
}

void test_dynamic()
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> double_point_type;
    typedef bg::model::polygon<double_point_type> polygon_type;
    typedef bg::model::box<double_point_type> box_type;

    polygon_type poly;
    std::string const polygon_li = "POLYGON((18 5,18 1,15 1,15 5,12 5,12 8,15 8,18 8,18 5))";
    bg::read_wkt(polygon_li, poly);

    box_type box;
    std::string const box_li = "BOX(0 0,2 2)";
    bg::read_wkt(box_li, box);

    auto apoly = bg::area(poly);
    auto abox = bg::area(box);

    boost::variant<polygon_type, box_type> v;
    v = poly;
    BOOST_CHECK_CLOSE(bg::area(v), apoly, 0.0001);
    v = box;
    BOOST_CHECK_CLOSE(bg::area(v), abox, 0.0001);

    boost::variant2::variant<polygon_type, box_type> v2;
    v2 = poly;
    BOOST_CHECK_CLOSE(bg::area(v2), apoly, 0.0001);
    v2 = box;
    BOOST_CHECK_CLOSE(bg::area(v2), abox, 0.0001);

    bg::model::geometry_collection<boost::variant<polygon_type, box_type> > gc;
    gc.push_back(poly);
    gc.push_back(box);
    BOOST_CHECK_CLOSE(bg::area(gc), apoly + abox, 0.0001);
}

int test_main(int, char* [])
{

    test_all<bg::model::point<int, 2, bg::cs::cartesian> >();
    test_all<bg::model::point<float, 2, bg::cs::cartesian> >();
    test_all<bg::model::point<double, 2, bg::cs::cartesian> >();

    typedef bg::model::point<double, 2, bg::cs::cartesian> pt_crt;
    typedef bg::model::point<double, 2, bg::cs::spherical_equatorial<bg::degree> > pt_sph;
    typedef bg::model::point<double, 2, bg::cs::geographic<bg::degree> > pt_geo;

    // mean Earth's radius^2
    double r2 = bg::math::sqr(bg::get_radius<0>(bg::srs::sphere<double>()));

    test_ccw<pt_crt>();
    test_ccw<pt_sph>();
    test_ccw<pt_geo>();

    test_open<pt_crt>(2.0);
    test_open<pt_sph>(24726179921.523518 / r2);
    test_open<pt_geo >(24615770547.825359);

    test_open_ccw<pt_crt>(2.0);
    test_open_ccw<pt_sph>(24726179921.523518 / r2);
    test_open_ccw<pt_geo >(24615770547.825359);

    test_poles_ccw<pt_crt>();
    test_poles_ccw<pt_sph>();
    test_poles_ccw<pt_geo>();

    test_large_integers();

    test_dynamic();

    test_accurate_sum_strategy();

    // test_empty_input<bg::model::d2::point_xy<int> >();

    return 0;
}
