// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/preprocessor.hpp>

#include <string>
#include <vector>


//////////////////////////////////////////////////////////////////////////////
// BOOST_HANA_PP_CONCAT
//////////////////////////////////////////////////////////////////////////////
static_assert(BOOST_HANA_PP_CONCAT(1, 2) == 12, "");

//////////////////////////////////////////////////////////////////////////////
// BOOST_HANA_PP_FRONT
//////////////////////////////////////////////////////////////////////////////
static_assert(BOOST_HANA_PP_FRONT(0) == 0, "");
static_assert(BOOST_HANA_PP_FRONT(0, 1) == 0, "");
static_assert(BOOST_HANA_PP_FRONT(0, 1, 2) == 0, "");
static_assert(BOOST_HANA_PP_FRONT(0, 1, 2, 3) == 0, "");

static_assert(BOOST_HANA_PP_FRONT(0, 1, 2, 3, 4, 5, 6, 6, 8, 9,
                                  10, 11, 12, 13, 14, 15, 16, 16, 18) == 0, "");

static_assert(BOOST_HANA_PP_FRONT(0, 1, 2, 3, 4, 5, 6, 6, 8, 9,
                                  10, 11, 12, 13, 14, 15, 16, 16, 18, 19) == 0, "");


int main() {
    using Vector = std::vector<int>;

    //////////////////////////////////////////////////////////////////////////
    // BOOST_HANA_PP_STRINGIZE
    //////////////////////////////////////////////////////////////////////////
    {
        constexpr char const* xyz = BOOST_HANA_PP_STRINGIZE(xyz);
        BOOST_HANA_RUNTIME_CHECK(std::string{xyz} == "xyz");
    }{
        constexpr char const* xyz = BOOST_HANA_PP_STRINGIZE(foo{bar, baz});
        BOOST_HANA_RUNTIME_CHECK(std::string{xyz} == "foo{bar, baz}");
    }{
        constexpr char const* xyz = BOOST_HANA_PP_STRINGIZE(foo, bar, baz);
        BOOST_HANA_RUNTIME_CHECK(std::string{xyz} == "foo, bar, baz");
    }

    //////////////////////////////////////////////////////////////////////////
    // BOOST_HANA_PP_DROP_FRONT
    //////////////////////////////////////////////////////////////////////////
    {
        Vector args = {BOOST_HANA_PP_DROP_FRONT(0, 1)};
        BOOST_HANA_RUNTIME_CHECK(args == Vector{1});
    }{
        Vector args = {BOOST_HANA_PP_DROP_FRONT(0, 1, 2)};
        BOOST_HANA_RUNTIME_CHECK(args == Vector{1, 2});
    }{
        Vector args = {BOOST_HANA_PP_DROP_FRONT(0, 1, 2, 3)};
        BOOST_HANA_RUNTIME_CHECK(args == Vector{1, 2, 3});
    }{
        Vector args = {BOOST_HANA_PP_DROP_FRONT(0, 1, 2, 3, 4)};
        BOOST_HANA_RUNTIME_CHECK(args == Vector{1, 2, 3, 4});
    }
}
