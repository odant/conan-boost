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
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>

#include <boost/program_options.hpp>

template <typename Polygon>
inline void make_polygon(Polygon& polygon, int count_x, int count_y, int index, int offset)
{
    typedef typename bg::point_type<Polygon>::type point_type;
    bg::exterior_ring(polygon).push_back(point_type(0, 0));
    bg::exterior_ring(polygon).push_back(point_type(0, count_y * 10));
    bg::exterior_ring(polygon).push_back(point_type(count_x * 10 + 10, count_y * 10));
    bg::exterior_ring(polygon).push_back(point_type(count_x * 10 + 10, 0));
    bg::exterior_ring(polygon).push_back(point_type(0, 0));

    for(int j = 0; j < count_x; ++j)
    {
        for(int k = 0; k < count_y; ++k)
        {
            polygon.inners().push_back(typename Polygon::inner_container_type::value_type());
            polygon.inners().back().push_back(point_type(offset + j * 10 + 1, k * 10 + 1));
            polygon.inners().back().push_back(point_type(offset + j * 10 + 7, k * 10 + 5 + index));
            polygon.inners().back().push_back(point_type(offset + j * 10 + 5 + index, k * 10 + 7));
            polygon.inners().back().push_back(point_type(offset + j * 10 + 1, k * 10 + 1));
        }
    }
    bg::correct(polygon);
}



template <typename Polygon>
void test_star_comb(int index, int count_x, int count_y, int offset, p_q_settings const& settings)
{
    Polygon p, q;

    make_polygon(p, count_x, count_y, 0, 0);
    make_polygon(q, count_x, count_y, 1, offset);

    std::ostringstream out;
    out << "interior_triangles" << index;
    test_overlay_p_q
        <
            Polygon,
            typename bg::coordinate_type<Polygon>::type
        >(out.str(), p, q, settings);
}


template <typename T, bool Clockwise, bool Closed>
void test_all(int count, int count_x, int count_y, int offset, p_q_settings const& settings)
{
    auto const t0 = std::chrono::high_resolution_clock::now();

    typedef bg::model::polygon
        <
            bg::model::d2::point_xy<T>, Clockwise, Closed
        > polygon;


    for(int i = 0; i < count; i++)
    {
        test_star_comb<polygon>(i, count_x, count_y, offset, settings);
    }
    auto const t = std::chrono::high_resolution_clock::now();
    auto const elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0).count();
    std::cout
        << " type: " << string_from_type<T>::name()
        << " time: " << elapsed_ms / 1000.0 << std::endl;
}

int main(int argc, char** argv)
{
    BoostGeometryWriteTestConfiguration();
    try
    {
        namespace po = boost::program_options;
        po::options_description description("=== interior_triangles ===\nAllowed options");

        int offset = 0;
        int count = 1;
        int count_x = 10;
        int count_y = 10;
        bool ccw = false;
        bool open = false;
        p_q_settings settings;

        description.add_options()
            ("help", "Help message")
            ("count", po::value<int>(&count)->default_value(1), "Number of tests")
            ("count_x", po::value<int>(&count_x)->default_value(10), "Triangle count in x-direction")
            ("count_y", po::value<int>(&count_y)->default_value(10), "Triangle count in y-direction")
            ("offset", po::value<int>(&offset)->default_value(0), "Offset of second triangle in x-direction")
            ("diff", po::value<bool>(&settings.also_difference)->default_value(false), "Include testing on difference")
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
            ("ccw", po::value<bool>(&ccw)->default_value(false), "Counter clockwise polygons")
            ("open", po::value<bool>(&open)->default_value(false), "Open polygons")
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

#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
        if (ccw && open)
        {
            test_all<default_test_type, false, false>(count, count_x, count_y, offset, settings);
        }
        else if (ccw)
        {
            test_all<default_test_type, false, true>(count, count_x, count_y, offset, settings);
        }
        else if (open)
        {
            test_all<default_test_type, true, false>(count, count_x, count_y, offset, settings);
        }
        else
#endif
        {
            test_all<default_test_type, true, true>(count, count_x, count_y, offset, settings);
        }
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
