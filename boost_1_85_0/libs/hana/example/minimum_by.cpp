// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/minimum.hpp>
#include <boost/hana/ordering.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(
        hana::minimum.by(hana::ordering(hana::length), hana::make_tuple(
            hana::make_tuple(),
            hana::make_tuple(1, '2'),
            hana::make_tuple(3.3, nullptr, 4)
        ))
        == hana::make_tuple()
    );
}
