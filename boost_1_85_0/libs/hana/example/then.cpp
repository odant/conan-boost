// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/equal.hpp>
#include <boost/hana/then.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


struct undefined { };
static_assert(
    hana::then(hana::make_tuple(undefined{}, undefined{}), hana::make_tuple(1, 2, 3))
        ==
    hana::make_tuple(
        1, 2, 3,
        1, 2, 3
    )
, "");

int main() { }
