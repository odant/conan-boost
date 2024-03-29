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

#include <test_overlay_p_q.hpp>
#include <common/make_random_generator.hpp>

#include <boost/program_options.hpp>

struct star_params
{
    int count; // points of ellipse, not of star
    double factor_1;
    double factor_2;
    double center_x;
    double center_y;
    double rotation;
    star_params(int c, double f1, double f2, double x, double y, double r = 0)
        : count(c)
        , factor_1(f1)
        , factor_2(f2)
        , center_x(x)
        , center_y(y)
        , rotation(r)
    {}
};



template <typename Polygon>
inline void make_star(Polygon& polygon, star_params const& p)
{
    typedef typename bg::point_type<Polygon>::type P;
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

    coordinate_type a1 = coordinate_type(p.factor_1) * half * dx;
    coordinate_type b1 = coordinate_type(p.factor_1) * half * dy;
    coordinate_type a2 = coordinate_type(p.factor_2) * half * dx;
    coordinate_type b2 = coordinate_type(p.factor_2) * half * dy;

    coordinate_type pi = boost::math::constants::pi<long double>();
    coordinate_type delta = pi * two / coordinate_type(p.count - 1);
    coordinate_type angle = coordinate_type(p.rotation) * delta;
    for (int i = 0; i < p.count - 1; i++, angle += delta)
    {
        bool even = i % 2 == 0;
        coordinate_type s = sin(angle);
        coordinate_type c = cos(angle);
        coordinate_type x = p.center_x + cx + (even ? a1 : a2) * s;
        coordinate_type y = p.center_y + cy + (even ? b1 : b2) * c;
        bg::exterior_ring(polygon).push_back(bg::make<P>(x, y));

    }
    bg::exterior_ring(polygon).push_back(bg::exterior_ring(polygon).front());
    bg::correct(polygon);
}


template <typename T, bool Clockwise, bool Closed>
void test_star_ellipse(int seed, int index, star_params const& par_p,
            star_params const& par_q, p_q_settings const& settings)
{
    typedef bg::model::d2::point_xy<T> point_type;
    typedef bg::model::polygon<point_type, Clockwise, Closed> polygon;

    polygon p, q;
    make_star(p, par_p);
    make_star(q, par_q);

    std::ostringstream out;
    out << "star_ellipse_" << seed << "_" << index;
    test_overlay_p_q<polygon, T>(out.str(), p, q, settings);
}

template <typename T, bool Clockwise, bool Closed>
void test_type(int seed, int count, p_q_settings const& settings)
{
    auto const t0 = std::chrono::high_resolution_clock::now();

    auto factor_generator = make_real_generator(seed, 0.3, 1.2);
    auto location_generator = make_real_generator(seed, -20.0, +20.0); // -25.0, +25.0
    auto rotation_generator = make_real_generator(seed, 0, 0.5);
    auto int_generator = make_real_generator(seed, 5, 15);

    for(int i = 0; i < count; i++)
    {
        test_star_ellipse<T, Clockwise, Closed>(seed, i + 1,
            star_params(int_generator() * 2 + 1,
                    factor_generator(), factor_generator(),
                    location_generator(), location_generator(), rotation_generator()),
            star_params(int_generator() * 2 + 1,
                    factor_generator(), factor_generator(),
                    location_generator(), location_generator(), rotation_generator()),
            settings);
    }
    auto const t = std::chrono::high_resolution_clock::now();
    auto const elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0).count();
    std::cout << std::endl
        << "type: " << string_from_type<T>::name()
        << " time: " << elapsed_ms / 1000.0 << std::endl;
}

template <bool Clockwise, bool Closed>
void test_all(std::string const& type, int seed, int count, p_q_settings settings)
{
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
    if (type == "float")
    {
        test_type<float, Clockwise, Closed>(seed, count, settings);
    }
    else if (type == "double")
#endif
    {
        test_type<double, Clockwise, Closed>(seed, count, settings);
    }
}


int main(int argc, char** argv)
{
    BoostGeometryWriteTestConfiguration();
    try
    {
        namespace po = boost::program_options;
        po::options_description description("=== random_ellipses_stars ===\nAllowed options");

        int count = 1;
        int seed = -1;
        std::string type = "double";
        bool ccw = false;
        bool open = false;
        p_q_settings settings;

        description.add_options()
            ("help", "Help message")
            ("seed", po::value<int>(&seed), "Initialization seed for random generator")
            ("count", po::value<int>(&count)->default_value(1), "Number of tests")
            ("diff", po::value<bool>(&settings.also_difference)->default_value(false), "Include testing on difference")
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
            ("ccw", po::value<bool>(&ccw)->default_value(false), "Counter clockwise polygons")
            ("open", po::value<bool>(&open)->default_value(false), "Open polygons")
            ("type", po::value<std::string>(&type)->default_value("double"), "Type (float,double)")
#endif
            ("verbose", po::value<bool>(&settings.verbose), "Verbose")
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
            test_all<false, false>(type, seed, count, settings);
        }
        else if (ccw)
        {
            test_all<false, true>(type, seed, count, settings);
        }
        else if (open)
        {
            test_all<true, false>(type, seed, count, settings);
        }
        else
#endif
        {
            test_all<true, true>(type, seed, count, settings);
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
