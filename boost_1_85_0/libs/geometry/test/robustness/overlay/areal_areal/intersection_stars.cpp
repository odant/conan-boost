// Boost.Geometry (aka GGL, Generic Geometry Library)
// Robustness Test

// Copyright (c) 2009-2021 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_GEOMETRY_NO_BOOST_TEST

#ifndef BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE
#define BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE
#endif

// NOTE: there is no randomness here. Count is to measure performance

#include <test_overlay_p_q.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/program_options.hpp>

template <typename Polygon>
inline void make_star(Polygon& polygon,
    int count, double factor1, double factor2, long double offset = 0)
{
    typedef typename bg::point_type<Polygon>::type p;
    typedef typename bg::select_most_precise
        <
            typename bg::coordinate_type<Polygon>::type,
            long double
        >::type coordinate_type;

    // Create star
    coordinate_type cx = 25.0;
    coordinate_type cy = 25.0;

    coordinate_type dx = 50.0;
    coordinate_type dy = 50.0;

    coordinate_type half = 0.5;
    coordinate_type two = 2.0;

    coordinate_type a1 = coordinate_type(factor1) * half * dx;
    coordinate_type b1 = coordinate_type(factor1) * half * dy;
    coordinate_type a2 = coordinate_type(factor2) * half * dx;
    coordinate_type b2 = coordinate_type(factor2) * half * dy;

    coordinate_type pi = boost::math::constants::pi<long double>();
    coordinate_type delta = pi * two / coordinate_type(count - 1);
    coordinate_type angle = coordinate_type(offset) * delta;
    for (int i = 0; i < count - 1; i++, angle += delta)
    {
        bool even = i % 2 == 0;
        coordinate_type s = sin(angle);
        coordinate_type c = cos(angle);
        coordinate_type x = cx + (even ? a1 : a2) * s;
        coordinate_type y = cy + (even ? b1 : b2) * c;
        bg::exterior_ring(polygon).push_back(bg::make<p>(x, y));

    }
    bg::exterior_ring(polygon).push_back(bg::exterior_ring(polygon).front());
}


template <typename T, typename CalculationType>
void test_star(int count, int min_points, int max_points, T rotation, p_q_settings const& settings)
{
    auto const t0 = std::chrono::high_resolution_clock::now();
    typedef bg::model::d2::point_xy<T> point_type;
    typedef bg::model::polygon<point_type> polygon;

    int n = 0;
    for (int c = 0; c < count; c++)
    {
        for (int i = min_points; i <= max_points; i++)
        {
            std::ostringstream out;
            out << "stars_" << c << "_" << i;

            polygon p;
            make_star(p, i * 2 + 1, 0.5, 1.0);
            polygon q;
            make_star(q, i * 2 + 1, 0.5, 1.0, rotation);

            if (! test_overlay_p_q
                <
                    polygon,
                    CalculationType
                >(out.str(), p, q, settings))
            {
                return;
            }
            n++;
        }
    }
    auto const t = std::chrono::high_resolution_clock::now();
    auto const elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0).count();
    std::cout
        << "polygons: " << n
        << " type: " << string_from_type<T>::name()
        << " time: " << elapsed_ms / 1000.0 << std::endl;
}

template <typename T, typename CalculationType>
void test_type(int count, int min_points, int max_points, T rotation, p_q_settings const& settings)
{
    test_star<T, CalculationType>(count, min_points, max_points, rotation, settings);
}

template <typename T>
void test_all(std::string const& type, int count, int min_points, int max_points, T rotation, p_q_settings settings)
{
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
    if (type == "float")
    {
        settings.tolerance = 1.0e-3;
        test_type<float, float>(count, min_points, max_points, rotation, settings);
    }
    else if (type == "double")
#endif
    {
        test_type<double, double>(count, min_points, max_points, rotation, settings);
    }
}

int main(int argc, char** argv)
{
    BoostGeometryWriteTestConfiguration();
    try
    {
        namespace po = boost::program_options;
        po::options_description description("=== recursive_polygons ===\nAllowed options");

        int count = 1;
        std::string type = "double";
        int min_points = 9;
        int max_points = 9;
        bool ccw = false;
        bool open = false;
        double rotation = 1.0e-13;
        p_q_settings settings;

        description.add_options()
            ("help", "Help message")
            ("count", po::value<int>(&count)->default_value(1), "Number of tests")
            ("diff", po::value<bool>(&settings.also_difference)->default_value(false), "Include testing on difference")
            ("min_points", po::value<int>(&min_points)->default_value(9), "Minimum number of points")
            ("max_points", po::value<int>(&max_points)->default_value(9), "Maximum number of points")
            ("rotation", po::value<double>(&rotation)->default_value(1.0e-13), "Rotation angle")
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
            ("ccw", po::value<bool>(&ccw)->default_value(false), "Counter clockwise polygons")
            ("open", po::value<bool>(&open)->default_value(false), "Open polygons")
            ("type", po::value<std::string>(&type)->default_value("double"), "Type (float,double)")
#endif
            ("wkt", po::value<bool>(&settings.wkt)->default_value(false), "Create a WKT of the inputs, for all tests")
            ("svg", po::value<bool>(&settings.svg)->default_value(false), "Create a SVG for all tests")
        ;

        po::variables_map varmap;
        po::store(po::parse_command_line(argc, argv, description), varmap);
        po::notify(varmap);

        if (varmap.count("help"))
        {
            std::cout << description << std::endl;
            return 1;
        }

        test_all(type, count, min_points, max_points, rotation, settings);
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Other exception" << std::endl;
    }
    return 0;
}
