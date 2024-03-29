#include <boost/config.hpp>

#if defined(BOOST_MSVC)
#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#endif

//
//  bind_dm2_test.cpp - data members, advanced uses
//
//  Copyright (c) 2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/bind/bind.hpp>
#include <boost/core/lightweight_test.hpp>
#include <string>

using namespace boost::placeholders;

//

struct X
{
    int m;
};

struct Y
{
    char m[ 64 ];
};

int main()
{
    X x = { 0 };
    X * px = &x;

    boost::bind< int& >( &X::m, _1 )( px ) = 42;

    BOOST_TEST( x.m == 42 );

    boost::bind< int& >( &X::m, boost::ref(x) )() = 17041;

    BOOST_TEST( x.m == 17041 );

    X const * pcx = &x;

    BOOST_TEST( boost::bind< long >( &X::m, _1 )( pcx ) == 17041L );
    BOOST_TEST( boost::bind< long >( &X::m, pcx )() == 17041L );

    Y y = { "test" };
    std::string v( "test" );

    BOOST_TEST( boost::bind< char const* >( &Y::m, &y )() == v );
    BOOST_TEST( boost::bind< std::string >( &Y::m, &y )() == v );

    return boost::report_errors();
}
