//=======================================================================
// Copyright 2001 University of Notre Dame.
// Author: Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <boost/core/lightweight_test.hpp>
#include <stdlib.h>

#include <boost/graph/undirected_dfs.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_archetypes.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/random.hpp>

#include <boost/random/mersenne_twister.hpp>

template < typename ColorMap, typename ParentMap, typename DiscoverTimeMap,
    typename FinishTimeMap >
class dfs_test_visitor
{
    typedef typename boost::property_traits< ColorMap >::value_type ColorValue;
    typedef typename boost::color_traits< ColorValue > Color;

public:
    dfs_test_visitor(
        ColorMap color, ParentMap p, DiscoverTimeMap d, FinishTimeMap f)
    : m_color(color)
    , m_parent(p)
    , m_discover_time(d)
    , m_finish_time(f)
    , m_time(0)
    {
    }

    template < class Vertex, class Graph >
    void initialize_vertex(Vertex u, Graph&)
    {
        BOOST_TEST(boost::get(m_color, u) == Color::white());
    }
    template < class Vertex, class Graph > void start_vertex(Vertex u, Graph&)
    {
        BOOST_TEST(boost::get(m_color, u) == Color::white());
    }
    template < class Vertex, class Graph >
    void discover_vertex(Vertex u, Graph&)
    {
        using namespace boost;
        BOOST_TEST(get(m_color, u) == Color::gray());
        BOOST_TEST(get(m_color, get(m_parent, u)) == Color::gray());

        put(m_discover_time, u, m_time++);
    }
    template < class Edge, class Graph > void examine_edge(Edge e, Graph& g)
    {
        using namespace boost;
        BOOST_TEST(get(m_color, source(e, g)) == Color::gray());
    }
    template < class Edge, class Graph > void tree_edge(Edge e, Graph& g)
    {
        using namespace boost;
        BOOST_TEST(get(m_color, target(e, g)) == Color::white());

        put(m_parent, target(e, g), source(e, g));
    }
    template < class Edge, class Graph > void back_edge(Edge e, Graph& g)
    {
        using namespace boost;
        BOOST_TEST(get(m_color, target(e, g)) == Color::gray());
    }
    template < class Edge, class Graph >
    void forward_or_cross_edge(Edge e, Graph& g)
    {
        using namespace boost;
        BOOST_TEST(get(m_color, target(e, g)) == Color::black());
    }
    template < class Edge, class Graph > void finish_edge(Edge e, Graph& g)
    {
        using namespace boost;
        BOOST_TEST((get(m_color, target(e, g)) == Color::gray())
            || (get(m_color, target(e, g)) == Color::black()));
    }
    template < class Vertex, class Graph > void finish_vertex(Vertex u, Graph&)
    {
        using namespace boost;
        BOOST_TEST(get(m_color, u) == Color::black());

        put(m_finish_time, u, m_time++);
    }

private:
    ColorMap m_color;
    ParentMap m_parent;
    DiscoverTimeMap m_discover_time;
    FinishTimeMap m_finish_time;
    typename boost::property_traits< DiscoverTimeMap >::value_type m_time;
};

template < typename Graph > struct dfs_test
{
    typedef boost::graph_traits< Graph > Traits;
    typedef typename Traits::vertices_size_type vertices_size_type;

    static void go(vertices_size_type max_V)
    {
        using namespace boost;
        typedef typename Traits::vertex_descriptor vertex_descriptor;
        typedef
            typename boost::property_map< Graph, boost::vertex_color_t >::type
                ColorMap;
        typedef
            typename boost::property_traits< ColorMap >::value_type ColorValue;
        typedef typename boost::color_traits< ColorValue > Color;
        typedef typename boost::property_map< Graph, boost::edge_color_t >::type
            EColorMap;
        typedef typename boost::property_traits< EColorMap >::value_type
            EColorValue;
        typedef typename boost::color_traits< EColorValue > EColor;

        vertices_size_type i, k;
        typename Traits::edges_size_type j;
        typename Traits::vertex_iterator vi, vi_end, ui, ui_end;
        typename Traits::edge_iterator ei, ei_end;

        boost::mt19937 gen;

        for (i = 0; i < max_V; ++i)
            for (j = 0; j < i * i; ++j)
            {
                Graph g;
                generate_random_graph(g, i, j, gen);

                ColorMap color = get(boost::vertex_color, g);
                EColorMap e_color = get(boost::edge_color, g);
                std::vector< vertex_descriptor > parent(num_vertices(g));
                for (k = 0; k < num_vertices(g); ++k)
                    parent[k] = k;
                std::vector< int > discover_time(num_vertices(g)),
                    finish_time(num_vertices(g));

                // Get vertex index map
                typedef typename boost::property_map< Graph,
                    boost::vertex_index_t >::const_type idx_type;
                idx_type idx = get(boost::vertex_index, g);

                typedef boost::iterator_property_map<
                    typename std::vector< vertex_descriptor >::iterator,
                    idx_type >
                    parent_pm_type;
                parent_pm_type parent_pm(parent.begin(), idx);
                typedef boost::iterator_property_map<
                    std::vector< int >::iterator, idx_type >
                    time_pm_type;
                time_pm_type discover_time_pm(discover_time.begin(), idx);
                time_pm_type finish_time_pm(finish_time.begin(), idx);

                dfs_test_visitor< ColorMap, parent_pm_type, time_pm_type,
                    time_pm_type >
                    vis(color, parent_pm, discover_time_pm, finish_time_pm);

                boost::undirected_dfs(
                    g, visitor(vis).color_map(color).edge_color_map(e_color));

                // all vertices should be black
                for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
                    BOOST_TEST(get(color, *vi) == Color::black());

                // all edges should be black
                for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
                    BOOST_TEST(get(e_color, *ei) == EColor::black());

                // check parenthesis structure of discover/finish times
                // See CLR p.480
                for (boost::tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui)
                    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end;
                         ++vi)
                    {
                        vertex_descriptor u = *ui, v = *vi;
                        if (u != v)
                        {
                            BOOST_TEST(finish_time[u] < discover_time[v]
                                || finish_time[v] < discover_time[u]
                                || (discover_time[v] < discover_time[u]
                                    && finish_time[u] < finish_time[v]
                                    && boost::is_descendant(u, v, parent_pm))
                                || (discover_time[u] < discover_time[v]
                                    && finish_time[v] < finish_time[u]
                                    && boost::is_descendant(v, u, parent_pm)));
                        }
                    }
            }
    }
};

// usage: undirected_dfs.exe [max-vertices=15]

int main(int argc, char* argv[])
{
    int max_V = 7;
    if (argc > 1)
        max_V = atoi(argv[1]);

    // Test undirected graphs.
    dfs_test< boost::adjacency_list< boost::vecS, boost::vecS,
        boost::undirectedS,
        boost::property< boost::vertex_color_t, boost::default_color_type >,
        boost::property< boost::edge_color_t, boost::default_color_type > > >::
        go(max_V);

    return boost::report_errors();
}
