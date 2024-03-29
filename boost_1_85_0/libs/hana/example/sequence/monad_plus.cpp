// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/append.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/empty.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
namespace hana = boost::hana;
using namespace std::string_literals;


BOOST_HANA_CONSTANT_CHECK(hana::empty<hana::tuple_tag>() == hana::make_tuple());

static_assert(hana::append(hana::make_tuple(1, '2', 3.3), nullptr)
                        == hana::make_tuple(1, '2', 3.3, nullptr), "");

int main() {
    BOOST_HANA_RUNTIME_CHECK(
        hana::concat(hana::make_tuple(1, '2', 3.3), hana::make_tuple("abcdef"s)) ==
        hana::make_tuple(1, '2', 3.3, "abcdef"s)
    );
}
