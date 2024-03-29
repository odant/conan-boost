// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


template <typename ...>
struct F { struct type; };

struct x0;
struct x1;
struct x2;
struct x3;

int main() {
    // tuple_t and initial state
    {
        auto f = hana::metafunction<F>;
        auto s = hana::type_c<struct initial_state>;
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<>, s, f),
            s
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0>, s, f),
            f(s, hana::type_c<x0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0, x1>, s, f),
            f(f(s, hana::type_c<x0>), hana::type_c<x1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0, x1, x2>, s, f),
            f(f(f(s, hana::type_c<x0>), hana::type_c<x1>), hana::type_c<x2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0, x1, x2, x3>, s, f),
            f(f(f(f(s, hana::type_c<x0>), hana::type_c<x1>), hana::type_c<x2>), hana::type_c<x3>)
        ));
    }

    // tuple_t and no initial state
    {
        auto f = hana::metafunction<F>;
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0>, f),
            hana::type_c<x0>
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0, x1>, f),
            f(hana::type_c<x0>, hana::type_c<x1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0, x1, x2>, f),
            f(f(hana::type_c<x0>, hana::type_c<x1>), hana::type_c<x2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::fold_left(hana::tuple_t<x0, x1, x2, x3>, f),
            f(f(f(hana::type_c<x0>, hana::type_c<x1>), hana::type_c<x2>), hana::type_c<x3>)
        ));
    }
}
