// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2014-2021.
// Modifications copyright (c) 2014-2021 Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle
// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_TEST_CONVEX_HULL_HPP
#define BOOST_GEOMETRY_TEST_CONVEX_HULL_HPP

#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/convex_hull.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <boost/geometry/algorithms/num_points.hpp>
#include <boost/geometry/algorithms/perimeter.hpp>

#include <boost/geometry/geometries/adapted/boost_variant.hpp>
#include <boost/geometry/geometries/geometry_collection.hpp>
#include <boost/geometry/geometries/linestring.hpp>

#include <boost/geometry/strategies/strategies.hpp>
#include <boost/geometry/strategies/spherical/side_by_cross_track.hpp>

#include <boost/geometry/strategy/cartesian/precise_area.hpp>
#include <boost/geometry/strategy/cartesian/side_by_triangle.hpp>
#include <boost/geometry/strategy/cartesian/side_non_robust.hpp>
#include <boost/geometry/strategy/cartesian/side_robust.hpp>
#include <boost/geometry/strategy/cartesian/precise_area.hpp>

#include <boost/geometry/io/wkt/wkt.hpp>

#include <boost/geometry/geometries/polygon.hpp>


struct robust_cartesian : boost::geometry::strategies::convex_hull::cartesian<>
{
    static auto side()
    {
        return boost::geometry::strategy::side::side_robust
            <
                double,
                boost::geometry::strategy::side::fp_equals_policy
            >();
    }
};

struct non_robust_cartesian_fast : boost::geometry::strategies::convex_hull::cartesian<>
{
    static auto side()
    {
        return boost::geometry::strategy::side::side_non_robust<>();
    }
};

struct non_robust_cartesian_sbt : boost::geometry::strategies::convex_hull::cartesian<>
{
    static auto side()
    {
        return boost::geometry::strategy::side::side_by_triangle<>();
    }
};

template <typename CalculationType = void>
struct sphrerical_side_by_cross_track : boost::geometry::strategies::convex_hull::spherical<>
{
    static auto side()
    {
        return boost::geometry::strategy::side::side_by_cross_track<>();
    }
};

struct precise_cartesian : boost::geometry::strategies::area::cartesian<>
{
    // Technically this should be enabled only for non-boxes
    template <typename Geometry>
    static auto area(Geometry const&)
    {
        return boost::geometry::strategy::area::precise_cartesian<>();
    }
};


template <typename Geometry, typename Hull, typename AreaStrategy>
void check_convex_hull(Geometry const& geometry, Hull const& hull,
                      std::size_t /*size_original*/, std::size_t size_hull,
                      double expected_area, double expected_perimeter,
                      bool reverse, AreaStrategy area_strategy)
{
    std::size_t n = bg::num_points(hull);

    BOOST_CHECK_MESSAGE(n == size_hull,
        "convex hull: " << bg::wkt(geometry)
        << " -> " << bg::wkt(hull)
        << " type "
        << (typeid(typename bg::coordinate_type<Hull>::type).name())
        << " -> Expected: " << size_hull
        << " detected: " << n);


    // We omit this check as it is not important for the hull algorithm
    // BOOST_CHECK(bg::num_points(geometry) == size_original);

    auto ah = bg::area(hull, area_strategy);

    if (reverse)
    {
        ah = -ah;
    }

    BOOST_CHECK_CLOSE(ah, expected_area, 0.001);

    if ( expected_perimeter >= 0 )
    {
        auto ph = bg::perimeter(hull);

        BOOST_CHECK_CLOSE(ph, expected_perimeter, 0.001);
    }
}


template
<
    typename Hull,
    typename Strategy,
    typename AreaStrategy
>
struct test_convex_hull
{
    template <typename Geometry>
    static void apply(Geometry const& geometry,
                      std::size_t size_original,
                      std::size_t size_hull_closed,
                      double expected_area,
                      double expected_perimeter,
                      bool )
    {
        static bool const is_hull_closed = bg::closure<Hull>::value != bg::open;
        std::size_t const size_hull = is_hull_closed ? size_hull_closed :
                                                       size_hull_closed - 1;

        Hull hull;
        bg::convex_hull(geometry, hull.outer(), Strategy());

        check_convex_hull(geometry, hull, size_original, size_hull,
            expected_area, expected_perimeter, false, AreaStrategy());

        bg::clear(hull);
        bg::convex_hull(geometry, hull, Strategy());

        using point_t = typename bg::point_type<Hull>::type;
        using var_t = boost::variant<Hull, bg::model::linestring<point_t>, point_t>;
        using gc_t = bg::model::geometry_collection<var_t>;

        var_t var;
        bg::convex_hull(geometry, var, Strategy());

        gc_t gc;
        bg::convex_hull(geometry, gc, Strategy());

        BOOST_CHECK(var.which() == gc[0].which());

        if (bg::detail::equals::equals_point_point(hull.outer()[0], hull.outer()[1], Strategy()))
            BOOST_CHECK(gc[0].which() == 2); // GC stores point
        else if (bg::detail::equals::equals_point_point(hull.outer()[0], hull.outer()[2], Strategy()))
            BOOST_CHECK(gc[0].which() == 1); // GC stores linestring
        else
            BOOST_CHECK(gc[0].which() == 0); // GC stores polygon
    }
};


template
<
    typename Hull,
    typename AreaStrategy
>
struct test_convex_hull<Hull, robust_cartesian, AreaStrategy>
{
    template <typename Geometry>
    static void apply(Geometry const& geometry,
                      std::size_t size_original,
                      std::size_t size_hull_closed,
                      double expected_area,
                      double expected_perimeter,
                      bool )
    {
        static bool const is_hull_closed = bg::closure<Hull>::value != bg::open;
        std::size_t const size_hull = is_hull_closed ? size_hull_closed :
                                                       size_hull_closed - 1;

        // Test version with ring as output
        Hull hull;
        bg::convex_hull(geometry, hull.outer());
        check_convex_hull(geometry, hull, size_original, size_hull, expected_area,
                          expected_perimeter, false, AreaStrategy());

        // Test version with polygon as output
        bg::clear(hull);
        bg::convex_hull(geometry, hull);
        check_convex_hull(geometry, hull, size_original, size_hull, expected_area,
                          expected_perimeter, false, AreaStrategy());

        // Test version with strategy
        bg::clear(hull);
        bg::convex_hull(geometry, hull.outer(), robust_cartesian());
        check_convex_hull(geometry, hull, size_original, size_hull, expected_area,
                          expected_perimeter, false, AreaStrategy());
    }
};


template
<
    typename Hull
>
struct test_convex_hull<Hull, robust_cartesian, precise_cartesian>
{
    template <typename Geometry>
    static void apply(Geometry const& geometry,
                      std::size_t size_original,
                      std::size_t size_hull_closed,
                      double expected_area,
                      double expected_perimeter,
                      bool )
    {
        static bool const is_hull_closed = bg::closure<Hull>::value != bg::open;
        std::size_t const size_hull = is_hull_closed ? size_hull_closed :
                                                       size_hull_closed - 1;

        // Test version with strategy
        Hull hull;
        bg::convex_hull(geometry, hull.outer(), robust_cartesian());

        check_convex_hull(geometry, hull, size_original, size_hull, expected_area,
                          expected_perimeter, false, precise_cartesian());
    }
};


template
<
    typename Geometry,
    typename Strategy,
    typename AreaStrategy,
    bool Clockwise,
    bool Closed
>
void test_geometry_order(std::string const& wkt,
                         std::size_t size_original,
                         std::size_t size_hull_closed,
                         double expected_area,
                         double expected_perimeter = -1.0)
{
    typedef bg::model::polygon
        <
            typename bg::point_type<Geometry>::type,
            Clockwise,
            Closed
        > hull_type;

    Geometry geometry;
    bg::read_wkt(wkt, geometry);

    test_convex_hull<hull_type, Strategy, AreaStrategy>::apply(geometry, size_original,
        size_hull_closed, expected_area, expected_perimeter, !Clockwise);

    using variant_t = boost::variant<Geometry>;

    variant_t v(geometry);
    test_convex_hull<hull_type, Strategy, AreaStrategy>::apply(v, size_original,
        size_hull_closed, expected_area, expected_perimeter, !Clockwise);

    bg::model::geometry_collection<variant_t> gc = { v };
    test_convex_hull<hull_type, Strategy, AreaStrategy>::apply(gc, size_original,
        size_hull_closed, expected_area, expected_perimeter, !Clockwise);
}

template
<
    typename Geometry,
    typename Strategy,
    typename AreaStrategy = typename bg::strategies::area::services::default_strategy
        <
            Geometry
        >::type
>
void test_geometry(std::string const& wkt,
                   std::size_t size_original,
                   std::size_t size_hull_closed,
                   double expected_area,
                   double expected_perimeter = -1.0)
{
    test_geometry_order<Geometry, Strategy, AreaStrategy, true, true>(
        wkt, size_original, size_hull_closed, expected_area, expected_perimeter);
    test_geometry_order<Geometry, Strategy, AreaStrategy, false, true>(
        wkt, size_original, size_hull_closed, expected_area, expected_perimeter);
    test_geometry_order<Geometry, Strategy, AreaStrategy, true, false>(
        wkt, size_original, size_hull_closed, expected_area, expected_perimeter);
    test_geometry_order<Geometry, Strategy, AreaStrategy, false, false>(
        wkt, size_original, size_hull_closed, expected_area, expected_perimeter);
}

template <class SGeometry, class GGeometry>
void test_geometry_sph_geo(std::string const& wkt,
                           std::size_t size_original,
                           std::size_t size_hull_closed,
                           double spherical_expected_area,
                           double geographic_expected_area)
{
    typedef boost::geometry::strategies::convex_hull::spherical<> SphericalStrategy;
    test_geometry<SGeometry, SphericalStrategy>(wkt, size_original,
        size_hull_closed, spherical_expected_area);

    typedef boost::geometry::strategies::convex_hull::geographic<> GeoStrategy;
    test_geometry<GGeometry, GeoStrategy>(wkt, size_original,
        size_hull_closed, geographic_expected_area);
}


template <typename Geometry>
void test_empty_input()
{
    Geometry geometry;
    bg::model::polygon
        <
            typename bg::point_type<Geometry>::type
        > hull;

    bg::convex_hull(geometry, hull);
    BOOST_CHECK_MESSAGE(bg::is_empty(hull), "Output convex hull should be empty" );
}


#endif
