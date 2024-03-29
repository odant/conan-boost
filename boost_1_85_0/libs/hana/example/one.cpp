// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/one.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::one<hana::integral_constant_tag<int>>() == hana::int_c<1>);
static_assert(hana::one<long>() == 1l, "");

int main() { }
