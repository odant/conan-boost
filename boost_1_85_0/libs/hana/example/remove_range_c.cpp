// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/equal.hpp>
#include <boost/hana/remove_range.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


constexpr auto xs = hana::make_tuple(0, '1', 2.2, 3u, 4, 5.5);

static_assert(hana::remove_range_c<2, 4>(xs) == hana::make_tuple(0, '1', 4, 5.5), "");

int main() { }
