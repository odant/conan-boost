
// Copyright 2017 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>
#include <utility>
#include <string>

using namespace boost::variant2;

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

struct X1
{
    int v;

    X1(): v(0) {}
    explicit X1(int v): v(v) {}
    X1(X1 const& r): v(r.v) {}
    X1(X1&& r): v(r.v) {}
    X1& operator=( X1 const& r ) { v = r.v; return *this; }
    X1& operator=( X1&& r ) { v = r.v; return *this; }
};

inline bool operator==( X1 const& a, X1 const& b ) { return a.v == b.v; }

STATIC_ASSERT( !std::is_nothrow_default_constructible<X1>::value );
STATIC_ASSERT( !std::is_nothrow_copy_constructible<X1>::value );
STATIC_ASSERT( !std::is_nothrow_move_constructible<X1>::value );
STATIC_ASSERT( !std::is_nothrow_copy_assignable<X1>::value );
STATIC_ASSERT( !std::is_nothrow_move_assignable<X1>::value );

struct X2
{
    int v;

    X2(): v(0) {}
    explicit X2(int v): v(v) {}
    X2(X2 const& r): v(r.v) {}
    X2(X2&& r): v(r.v) {}
    X2& operator=( X2 const& r ) { v = r.v; return *this; }
    X2& operator=( X2&& r ) { v = r.v; return *this; }
};

inline bool operator==( X2 const& a, X2 const& b ) { return a.v == b.v; }

STATIC_ASSERT( !std::is_nothrow_default_constructible<X2>::value );
STATIC_ASSERT( !std::is_nothrow_copy_constructible<X2>::value );
STATIC_ASSERT( !std::is_nothrow_move_constructible<X2>::value );
STATIC_ASSERT( !std::is_nothrow_copy_assignable<X2>::value );
STATIC_ASSERT( !std::is_nothrow_move_assignable<X2>::value );

struct Y1
{
    int v = 1;

    Y1() = default;
    Y1(Y1 const&) = delete;
    Y1(Y1&&) = delete;
};

STATIC_ASSERT( !std::is_copy_constructible<Y1>::value );
STATIC_ASSERT( !std::is_move_constructible<Y1>::value );

struct Y2
{
    int v = 2;

    Y2() = default;
    Y2(Y2 const&) = delete;
    Y2(Y2&&) = delete;
};

STATIC_ASSERT( !std::is_copy_constructible<Y2>::value );
STATIC_ASSERT( !std::is_move_constructible<Y2>::value );

struct Z1
{
    static int instances;

    int v = 1;

    Z1() { ++instances; }
    ~Z1() { --instances; }

    Z1(Z1 const&) = delete;
    Z1(Z1&&) = delete;
};

int Z1::instances = 0;

struct Z2
{
    static int instances;

    int v = 2;

    Z2() { ++instances; }
    ~Z2() { --instances; }

    Z2(Z2 const&) = delete;
    Z2(Z2&&) = delete;
};

int Z2::instances = 0;

int main()
{
    {
        variant<int> v;
        BOOST_TEST_EQ( get<0>(v), 0 );

        v.emplace<0>( 1 );
        BOOST_TEST_EQ( get<0>(v), 1 );

        v.emplace<0>();
        BOOST_TEST_EQ( get<0>(v), 0 );
    }

    {
        variant<int, float> v;
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 0 );

        v.emplace<0>( 1 );
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 1 );

        v.emplace<1>( 3.14f );
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), 3.14f );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), 0 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 0 );
    }

    {
        variant<int, int, float, std::string> v;
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 0 );

        v.emplace<0>( 1 );
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 1 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), 0 );

        v.emplace<1>( 1 );
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), 1 );

        v.emplace<2>( 3.14f );
        BOOST_TEST_EQ( v.index(), 2 );
        BOOST_TEST_EQ( get<2>(v), 3.14f );

        v.emplace<2>();
        BOOST_TEST_EQ( v.index(), 2 );
        BOOST_TEST_EQ( get<2>(v), 0 );

        v.emplace<3>( "s1" );
        BOOST_TEST_EQ( v.index(), 3 );
        BOOST_TEST_EQ( get<3>(v), std::string("s1") );

        v.emplace<3>( "s2" );
        BOOST_TEST_EQ( v.index(), 3 );
        BOOST_TEST_EQ( get<3>(v), std::string("s2") );

        v.emplace<3>();
        BOOST_TEST_EQ( v.index(), 3 );
        BOOST_TEST_EQ( get<3>(v), std::string() );

        v.emplace<3>( { 'a', 'b' } );
        BOOST_TEST_EQ( v.index(), 3 );
        BOOST_TEST_EQ( get<3>(v), (std::string{ 'a', 'b'}) );

        v.emplace<3>( { 'c', 'd' }, std::allocator<char>() );
        BOOST_TEST_EQ( v.index(), 3 );
        BOOST_TEST_EQ( get<3>(v), (std::string{ 'c', 'd'}) );
    }

    {
        variant<X1, X2> v;
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 0 );

        v.emplace<0>( 1 );
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );

        v.emplace<1>( 2 );
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 2 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 0 );

        v.emplace<0>( 4 );
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 4 );

        v.emplace<1>();

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 0 );

        v.emplace<1>( 6 );
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 6 );

        v.emplace<0>( 3 );
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 3 );

        v.emplace<0>( 4 );
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 4 );
    }

    {
        variant<Y1, Y2> v;
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 2 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 2 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );
    }

    {
        variant<Z1, Z2> v;
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );
        BOOST_TEST_EQ( Z1::instances, 1 );
        BOOST_TEST_EQ( Z2::instances, 0 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );
        BOOST_TEST_EQ( Z1::instances, 1 );
        BOOST_TEST_EQ( Z2::instances, 0 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 2 );
        BOOST_TEST_EQ( Z1::instances, 0 );
        BOOST_TEST_EQ( Z2::instances, 1 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v).v, 2 );
        BOOST_TEST_EQ( Z1::instances, 0 );
        BOOST_TEST_EQ( Z2::instances, 1 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v).v, 1 );
        BOOST_TEST_EQ( Z1::instances, 1 );
        BOOST_TEST_EQ( Z2::instances, 0 );
    }

    BOOST_TEST_EQ( Z1::instances, 0 );
    BOOST_TEST_EQ( Z2::instances, 0 );

    {
        variant<monostate, monostate> v;
        BOOST_TEST_EQ( v.index(), 0 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
    }

    {
        variant<monostate, int> v;
        BOOST_TEST_EQ( v.index(), 0 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );

        v.emplace<1>();
        BOOST_TEST_EQ( v.index(), 1 );

        v.emplace<0>();
        BOOST_TEST_EQ( v.index(), 0 );
    }

    return boost::report_errors();
}
