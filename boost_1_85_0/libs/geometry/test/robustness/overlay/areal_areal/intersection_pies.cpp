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

#include <sstream>
#include <chrono>

#include <boost/program_options.hpp>

template <typename Polygon, typename T>
inline void make_pie(Polygon& polygon,
    int count, int offset, int offset_y, T factor1, int total_segment_count = 36)
{
    typedef typename bg::point_type<Polygon>::type p;
    typedef typename bg::select_most_precise
        <
            typename bg::coordinate_type<Polygon>::type,
            long double
        >::type coordinate_type;

    // Create pie
    coordinate_type cx = 2500.0;
    coordinate_type cy = 2500.0 + offset_y;

    bg::exterior_ring(polygon).push_back(bg::make<p>(int(cx), int(cy)));

    coordinate_type const dx = 5000.0;
    coordinate_type const dy = 5000.0;

    coordinate_type const half = 0.5;
    coordinate_type const two = 2.0;

    coordinate_type const a = coordinate_type(factor1) * half * dx;
    coordinate_type const b = coordinate_type(factor1) * half * dy;

    coordinate_type const pi = boost::math::constants::pi<long double>();
    coordinate_type const delta = pi * two / total_segment_count;
    coordinate_type angle = coordinate_type(offset) * delta;
    for (int i = 0; i < count; i++, angle += delta)
    {
        coordinate_type const s = sin(angle);
        coordinate_type const c = cos(angle);
        coordinate_type const x = cx + a * s;
        coordinate_type const y = cy + b * c;
        bg::exterior_ring(polygon).push_back(bg::make<p>(int(x), int(y)));
    }
    bg::exterior_ring(polygon).push_back(bg::make<p>(int(cx), int(cy)));
    bg::correct(polygon);
}


/*
sql server

TWO INNERS
1a) select geometry::STGeomFromText('POLYGON((0 0,0 10,10 10,10 0,0 0),(4 4,5 5,4 6,4 4),(6 6,5 5,6 4,6 6)) ', 0).STIsValid();  -> valid
same:
1b) POLYGON((0 0,0 10,10 10,10 0,0 0),(4 4,5 5,6 4,6 6,5 5,4 6,4 4))   -> valid

I-E tangency IET
2a) select geometry::STGeomFromText('POLYGON((0 0,0 10,10 10,10 0,0 0),(5 0,6 1,4 1,5 0))', 0).STIsValid(); -> valid
same by Self tangency ST
2b) select geometry::STGeomFromText('POLYGON((0 0,0 10,10 10,10 0,5 0,6 1,4 1,5 0, 0 0))', 0).STIsValid(); -> valid

Two inners all tangent
3a) POLYGON((0 0,0 10,10 10,10 0,0 0),(5 0,6 1,4 1,5 0),(5 1,6 2,4 2,5 1)) -> valid

postgis:
1a) select ST_IsValid(ST_GeomFromText('POLYGON((0 0,0 10,10 10,10 0,0 0),(4 4,5 5,4 6,4 4),(6 6,5 5,6 4,6 6))', 0)); -> valid
1b) POLYGON((0 0,0 10,10 10,10 0,0 0),(4 4,5 5,6 4,6 6,5 5,4 6,4 4))   -> NOT valid

2a) POLYGON((0 0,0 10,10 10,10 0,0 0),(5 0,6 1,4 1,5 0)) -> valid
2b) POLYGON((0 0,0 10,10 10,10 0,5 0,6 1,4 1,5 0, 0 0)) -> NOT valid (?)

3a) POLYGON((0 0,0 10,10 10,10 0,0 0),(5 0,6 1,4 1,5 0),(5 1,6 2,4 2,5 1)) -> valid


*/

template <typename Polygon>
inline void holify(Polygon& polygon)
{
    typedef typename bg::point_type<Polygon>::type point_type;

    Polygon p;
    bg::exterior_ring(p).push_back(bg::make<point_type>(0, 0));
    bg::exterior_ring(p).push_back(bg::make<point_type>(0, 5000));
    bg::exterior_ring(p).push_back(bg::make<point_type>(5000, 5000));
    bg::exterior_ring(p).push_back(bg::make<point_type>(5000, 0));
    bg::exterior_ring(p).push_back(bg::make<point_type>(0, 0));
    bg::interior_rings(p).push_back(bg::exterior_ring(polygon));
    bg::correct(p);

    polygon = p;
}

template <typename MultiPolygon>
inline void holify_multi(MultiPolygon& multi_polygon)
{
    typedef typename bg::point_type<MultiPolygon>::type point_type;

    typename boost::range_value<MultiPolygon>::type p;

    bg::exterior_ring(p).push_back(bg::make<point_type>(0, 0));
    bg::exterior_ring(p).push_back(bg::make<point_type>(0, 5000));
    bg::exterior_ring(p).push_back(bg::make<point_type>(5000, 5000));
    bg::exterior_ring(p).push_back(bg::make<point_type>(5000, 0));
    bg::exterior_ring(p).push_back(bg::make<point_type>(0, 0));

    for (int i = 0; i < multi_polygon.size(); i++)
    {
        bg::interior_rings(p).push_back(bg::exterior_ring(multi_polygon[i]));
    }

    bg::correct(p);

    multi_polygon.clear();
    multi_polygon.push_back(p);
}



template <typename T, bool Clockwise, bool Closed>
void test_pie(int total_segment_count, T factor_p, T factor_q,
            bool multi, bool single_selftangent, p_q_settings const& settings)
{
    auto const t0 = std::chrono::high_resolution_clock::now();
    typedef bg::model::d2::point_xy<T> point_type;
    typedef bg::model::polygon<point_type, Clockwise, Closed> polygon;
    typedef bg::model::multi_polygon<polygon> multi_polygon;

    int good_count = 0;
    int bad_count = 0;

    for (int a = 2; a < total_segment_count; a++)
    {
        polygon p;
        make_pie(p, a, 0, 0, factor_p, total_segment_count);

        //holify(p);

        for (int b = 2; b < total_segment_count; b++)
        {
            for (int offset = 1; offset < total_segment_count; offset++)
            {
                //for (int y = 0; y <= 2500; y += 500)
                int y = 0;
                {
                    polygon q;
                    make_pie(q, b, offset, y, factor_q, total_segment_count);

                    if (! multi)
                    {
                        //holify(q);

                        std::ostringstream out;
                        out << "pie_" << a << "_" << b << "_" << offset << "_" << y;
                        if (test_overlay_p_q<polygon, T>(out.str(), p, q, settings))
                        {
                            good_count++;
                        }
                        else
                        {
                            bad_count++;
                        }
                    }
                    else
                    {
                        int left = total_segment_count - b - 2;
                        //std::cout << a << " " << b << " " << left << std::endl;
                        for (int c = 2; c < left; c++)
                        {
                            polygon q2;
                            make_pie(q2, c, offset + b + 1, y, factor_q, total_segment_count);

                            std::ostringstream out;
                            out << "pie_" << a << "_" << b << "_" << offset << "_" << y
                                << "_" << c
                                ;

                            bool good = false;

                            // Represent as either multi-polygon, or as single-self-touching-polygon (INVALID)
                            if (single_selftangent)
                            {
                                polygon q1 = q;
                                for (unsigned int i = 1; i < q2.outer().size(); i++)
                                {
                                    q1.outer().push_back(q2.outer()[i]);
                                }
                                //holify(q1);
                                good = test_overlay_p_q<polygon, T>(out.str(), p, q1, settings);
                            }
                            else
                            {
                                multi_polygon mq;
                                mq.push_back(q);
                                mq.push_back(q2);
                                //holify_multi(mq);
                                good = test_overlay_p_q<polygon, T>(out.str(), p, mq, settings);
                            }

                            if (good)
                            {
                                good_count++;
                            }
                            else
                            {
                                bad_count++;
                            }
                        }
                    }
                }
            }
        }
    }
    auto const t = std::chrono::high_resolution_clock::now();
    auto const elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0).count();
    std::cout
        << " time: " << elapsed_ms / 1000.0 << std::endl
        << "Good: " << good_count << std::endl
        << "Bad: " << bad_count << std::endl;
}


template <typename T, bool Clockwise, bool Closed>
void test_all(bool multi, bool single_selftangent, p_q_settings const& settings)
{
    test_pie<T, Clockwise, Closed>(24, 0.55, 0.45, multi, single_selftangent, settings);
}

int main(int argc, char** argv)
{
    BoostGeometryWriteTestConfiguration();
    try
    {
        namespace po = boost::program_options;
        po::options_description description("=== intersection_pies ===\nAllowed options");

        p_q_settings settings;
        bool multi = false;
        bool ccw = false;
        bool open = false;
        bool single_selftangent = false; // keep false, true does not work!

        description.add_options()
            ("help", "Help message")
            ("multi", po::value<bool>(&multi)->default_value(false), "Multiple tangencies at one point")
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

        // template par's are: CoordinateType, Clockwise, Closed
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
        if (ccw && open)
        {
            test_all<default_test_type, false, false>(multi, single_selftangent, settings);
        }
        else if (ccw)
        {
            test_all<default_test_type, false, true>(multi, single_selftangent, settings);
        }
        else if (open)
        {
            test_all<default_test_type, true, false>(multi, single_selftangent, settings);
        }
        else
#endif
        {
            test_all<default_test_type, true, true>(multi, single_selftangent, settings);
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
