// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/orderable.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto ints = hana::make_tuple(
        hana::int_c<-10>,
        hana::int_c<-2>,
        hana::int_c<0>,
        hana::int_c<1>,
        hana::int_c<3>,
        hana::int_c<4>
    );

    hana::test::TestOrderable<hana::integral_constant_tag<int>>{ints};
}
