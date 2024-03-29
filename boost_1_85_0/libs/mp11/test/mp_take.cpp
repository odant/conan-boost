
//  Copyright 2015 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt


#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <boost/mp11/integral.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>
#include <tuple>
#include <utility>

struct X1 {};
struct X2 {};
struct X3 {};
struct X4 {};
struct X5 {};

int main()
{
    using boost::mp11::mp_list;
    using boost::mp11::mp_take;
    using boost::mp11::mp_take_c;
    using boost::mp11::mp_size_t;

    {
        using L1 = mp_list<>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L1, 0>, L1>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L1, mp_size_t<0>>, L1>));

        using L2 = mp_list<X1, X2, X3, X4, X5>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 0>, mp_list<>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 1>, mp_list<X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 2>, mp_list<X1, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 3>, mp_list<X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 4>, mp_list<X1, X2, X3, X4>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 5>, mp_list<X1, X2, X3, X4, X5>>));

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<0>>, mp_list<>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<1>>, mp_list<X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<2>>, mp_list<X1, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<3>>, mp_list<X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<4>>, mp_list<X1, X2, X3, X4>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<5>>, mp_list<X1, X2, X3, X4, X5>>));

        using L3 = mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4, X5, X1, X2, X3, X4, X5, X1, X2, X3, X4, X5>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3,  6>, mp_list<X1, X2, X3, X4, X5, X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3,  7>, mp_list<X1, X2, X3, X4, X5, X1, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3,  8>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3,  9>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3, 10>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4, X5>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3, 11>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4, X5, X1>>));

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t< 6>>, mp_list<X1, X2, X3, X4, X5, X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t< 7>>, mp_list<X1, X2, X3, X4, X5, X1, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t< 8>>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t< 9>>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t<10>>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4, X5>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t<11>>, mp_list<X1, X2, X3, X4, X5, X1, X2, X3, X4, X5, X1>>));
    }

    {
        using L1 = std::tuple<>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L1, 0>, L1>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L1, mp_size_t<0>>, L1>));

        using L2 = std::tuple<X1, X2, X3, X4, X5>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 0>, std::tuple<>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 1>, std::tuple<X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 2>, std::tuple<X1, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 3>, std::tuple<X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 4>, std::tuple<X1, X2, X3, X4>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L2, 5>, std::tuple<X1, X2, X3, X4, X5>>));

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<0>>, std::tuple<>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<1>>, std::tuple<X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<2>>, std::tuple<X1, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<3>>, std::tuple<X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<4>>, std::tuple<X1, X2, X3, X4>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L2, mp_size_t<5>>, std::tuple<X1, X2, X3, X4, X5>>));

        using L3 = std::tuple<X1, X2, X3, X4, X5, X1, X2, X3, X4, X5, X1, X2, X3, X4, X5, X1, X2, X3, X4, X5>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L3, 8>, std::tuple<X1, X2, X3, X4, X5, X1, X2, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L3, mp_size_t<9>>, std::tuple<X1, X2, X3, X4, X5, X1, X2, X3, X4>>));
    }

    {
        using L1 = std::pair<X1, X2>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take_c<L1, 2>, L1>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_take<L1, mp_size_t<2>>, L1>));
    }

    return boost::report_errors();
}
