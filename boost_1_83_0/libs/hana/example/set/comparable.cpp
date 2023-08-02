// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(
        hana::make_set(hana::int_c<0>, hana::type_c<char>, hana::int_c<1>)
            ==
        hana::make_set(hana::int_c<1>, hana::int_c<0>, hana::type_c<char>)
    );

    BOOST_HANA_CONSTANT_CHECK(
        hana::make_set(hana::int_c<0>, hana::type_c<char>)
            !=
        hana::make_set(hana::int_c<1>)
    );
}
