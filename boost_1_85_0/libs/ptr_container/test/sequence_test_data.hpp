//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2003-2005. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#include "test_data.hpp"
#include <boost/ptr_container/detail/ptr_container_disable_deprecated.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/bind/bind.hpp>

#if defined(BOOST_PTR_CONTAINER_DISABLE_DEPRECATED)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

template< typename C, typename B, typename T >
void reversible_container_test();

template< class IntContainer >
void algorithms_test();

template< typename C, typename B, typename T >
void reversible_container_test()
{
    using namespace boost;
    
    BOOST_TEST_MESSAGE( "starting reversible container test" ); 
    enum { max_cnt = 10 };
    C  c;
    set_capacity<C>()( c );
    BOOST_CHECK( c.size() == 0 );
    c.push_back( new T );
    BOOST_CHECK( c.size() == 1 );

    const C c2_dummy( c.begin(), c.end() );
    BOOST_CHECK_EQUAL( c2_dummy.size(), c.size() );
    const C c2( c.clone() );
    BOOST_CHECK_EQUAL( c2.size(), c.size() );
    
    C  c3( c.begin(), c.end() );
    set_capacity<C>()( c3 );
    BOOST_CHECK_EQUAL( c.size(), c3.size() );

    c.assign( c3.begin(), c3.end() );
    BOOST_CHECK_EQUAL( c.size(), c3.size() );
        
    c.assign( c3 );
    set_capacity<C>()( c );
    BOOST_TEST_MESSAGE( "finished construction test" ); 

    C a_copy( c );
    BOOST_CHECK_EQUAL( a_copy.size(), c.size() );
    a_copy = a_copy;
    BOOST_CHECK_EQUAL( a_copy.size(), c.size() );
    a_copy.clear();
    a_copy = a_copy;
    BOOST_CHECK( a_copy.empty() );
    BOOST_CHECK( !c.empty() );
    BOOST_TEST_MESSAGE( "finished copying test" ); 

    BOOST_DEDUCED_TYPENAME C::allocator_type alloc        = c.get_allocator();
    hide_warning(alloc);
    BOOST_DEDUCED_TYPENAME C::iterator i                  = c.begin();
    BOOST_DEDUCED_TYPENAME C::const_iterator ci           = c2.begin();
    BOOST_DEDUCED_TYPENAME C::iterator i2                 = c.end();
    hide_warning(i2);
    BOOST_DEDUCED_TYPENAME C::const_iterator ci2          = c2.begin();
    hide_warning(ci2);
    BOOST_DEDUCED_TYPENAME C::reverse_iterator ri         = c.rbegin();
    hide_warning(ri);
    BOOST_DEDUCED_TYPENAME C::const_reverse_iterator cri  = c2.rbegin();
    hide_warning(cri);
    BOOST_DEDUCED_TYPENAME C::reverse_iterator rv2        = c.rend();
    hide_warning(rv2);
    BOOST_DEDUCED_TYPENAME C::const_reverse_iterator cvr2 = c2.rend();
    hide_warning(cvr2);
    i  = c.rbegin().base();
    ci = c2.rbegin().base();
    i  = c.rend().base();
    ci = c2.rend().base();
    BOOST_CHECK_EQUAL( std::distance( c.rbegin(), c.rend() ),
                       std::distance( c.begin(), c.end() ) );
                         
    BOOST_TEST_MESSAGE( "finished iterator test" ); 

    BOOST_DEDUCED_TYPENAME C::size_type s                 = c.size();
    hide_warning(s);
    BOOST_DEDUCED_TYPENAME C::size_type s2                = c.max_size();
    hide_warning(s2);
    c.push_back( new T );
    std::size_t size = 2u;
#ifndef BOOST_NO_AUTO_PTR
    c.push_back( std::auto_ptr<T>( new T ) );
    ++size;
#endif
#ifndef BOOST_NO_CXX11_SMART_PTR
    c.push_back( std::unique_ptr<T>( new T ) );
    ++size;
#endif
    bool b                                                = c.empty();
    BOOST_CHECK( !c.empty() );
    b                                                     = is_null( c.begin() );
    BOOST_CHECK( b == false );
    BOOST_DEDUCED_TYPENAME C::reference r                 = c.front();
    hide_warning(r);
    BOOST_DEDUCED_TYPENAME C::const_reference cr          = c2.front();
    hide_warning(cr);
    BOOST_DEDUCED_TYPENAME C::reference r2                = c.back();
    hide_warning(r2);
    BOOST_DEDUCED_TYPENAME C::const_reference cr2         = c2.back();
    hide_warning(cr2);
    BOOST_TEST_MESSAGE( "finished accessors test" ); 
    
    c.push_back( new T );
    ++size;
    BOOST_CHECK_EQUAL( c.size(), size );

    c.pop_back(); 
    BOOST_CHECK( !c.empty() );
    c.insert( c.end(), new T );
#ifndef BOOST_NO_AUTO_PTR
    std::auto_ptr<T> ap(new T);
    c.insert( c.end(), ap );
    ++size;
#endif
#ifndef BOOST_NO_CXX11_SMART_PTR
    std::unique_ptr<T> up( new T );
    c.insert( c.end(), std::move( up ) );
    ++size;
#endif
    BOOST_CHECK_EQUAL( c.size(), size );

#if defined(BOOST_NO_SFINAE) || defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
#else
    c.insert( c.end(), c3 );
#endif    
    c3.insert( c3.end(), c.begin(), c.end() ); 
    c.erase( c.begin() );
    c3.erase( c3.begin(), c3.end() );
    c3.erase( boost::make_iterator_range(c3) );
    BOOST_CHECK( c3.empty() );
    BOOST_CHECK( !c.empty() );
    c.swap( c3 );
    BOOST_CHECK( !c3.empty() );
    c3.clear();
    BOOST_CHECK( c3.empty() );
    C c4;
    c4.swap(c3);
#ifdef BOOST_NO_SFINAE
#else
    swap(c4,c3);
#endif    
    BOOST_TEST_MESSAGE( "finished modifiers test" ); 
             
    c.push_back( new T ); c.push_back( new T ); c.push_back( new T ); 
    typedef BOOST_DEDUCED_TYPENAME C::auto_type auto_type;

#ifdef BOOST_NO_SFINAE
#else
    auto_type ptr       = c.release( c.begin() );
#endif    
#ifndef BOOST_NO_AUTO_PTR
    std::auto_ptr<C> ap2   = c.release();
#else
    std::unique_ptr<C> up2 = c.release();
#endif
    c                   = c2.clone();
    BOOST_CHECK( !c.empty() );
    auto_type ptr2      = c.replace( c.begin(), new T );
#ifndef BOOST_NO_AUTO_PTR
    ptr2                = c.replace( c.begin(), std::auto_ptr<T>( new T ) );
#endif
#ifndef BOOST_NO_CXX11_SMART_PTR
    ptr2                = c.replace( c.begin(), std::unique_ptr<T>( new T ) );
#endif
    BOOST_TEST_MESSAGE( "finished release/clone/replace test" ); 
                     
    c3.push_back( new T );
    c3.push_back( new T );
    c3.push_back( new T );
    c. BOOST_NESTED_TEMPLATE transfer<C>( c.begin(), c3.begin(), c3 );
    c. BOOST_NESTED_TEMPLATE transfer<C>( c.end(), c3.begin(), c3.end(), c3 );
#ifdef BOOST_NO_SFINAE
#else    
    c. BOOST_NESTED_TEMPLATE transfer<C>( c.end(), boost::make_iterator_range( c3 ), c3 );    
    BOOST_CHECK( c3.empty() );
    BOOST_CHECK( !c.empty() );
#endif    
    c3. BOOST_NESTED_TEMPLATE transfer<C>( c3.begin(), c );
    BOOST_CHECK( !c3.empty() );
    BOOST_CHECK( c.empty() );
    BOOST_TEST_MESSAGE( "finished transfer test" );  

    c3.resize( 0u );
    BOOST_CHECK( c3.empty() );
    c3.resize( 10u );
    BOOST_CHECK_EQUAL( c3.size(), 10u );
    c3.resize( 12u, &*c3.begin() );
    BOOST_CHECK_EQUAL( c3.size(), 12u );
    BOOST_TEST_MESSAGE( "finished resize test" );  
    
}



template< class CDerived, class CBase, class T >
void test_transfer()
{
    BOOST_TEST_MESSAGE( "starting transfer test" );
    CDerived from;
    CBase    to;

    set_capacity<CDerived>()( from );
    set_capacity<CBase>()( to );

    from.push_back( new T );
    from.push_back( new T );
    to. BOOST_NESTED_TEMPLATE transfer<CDerived>( to.end(), from );
    BOOST_TEST_MESSAGE( "finished transfer test" );
}



#include <boost/assign/list_inserter.hpp>
#include <iostream>

template< class Compare, class C >
bool is_sorted( const C& c )
{
    if( c.size() < 2 )
        return true;
    
    typename C::const_iterator prev = c.begin(),
                               e = c.end(),
                               next = prev;
    Compare pred;
    for( ; next != e ; )
    {
        prev = next;
        ++next;

        if( next == c.end() )
            return true;
        
        if( !pred(*prev,*next) )
            return false;
    }
    return true;
}




struct equal_to_int
{
    int i_;
    
    equal_to_int( int i ) : i_(i)
    { }
    
    bool operator()( int i ) const
    {
        return i_ == i; 
    }
};


template< class IntContainer >
void random_access_algorithms_test()
{
    BOOST_TEST_MESSAGE( "starting random accessors algorithms test" );

    IntContainer c;
    set_capacity<IntContainer>()( c );
    assign::push_back( c )
                    ( new int(1) )
                    ( new int(3) )
                    ( new int(6) )
                    ( new int(7) )
                    ( new int(2) )
                    ( new int(2) )
                    ( new int(0) )
                    ( new int(6) )
                    ( new int(3) );
    c.sort();
    BOOST_CHECK( is_sorted< std::less_equal<int> >( c ) );
    BOOST_CHECK_EQUAL( c.size(), 9u );
   
    c.unique();
    BOOST_CHECK_EQUAL( c.size(), 6u );
#ifdef PTR_LIST_TEST
    BOOST_CHECK( c.front() == 0 );
    BOOST_CHECK( c.back() == 7 );
#else
    BOOST_CHECK( c[0] == 0 );
    BOOST_CHECK( c[1] == 1 );
    BOOST_CHECK( c[2] == 2 );
    BOOST_CHECK( c[3] == 3 );
    BOOST_CHECK( c[4] == 6 );
    BOOST_CHECK( c[5] == 7 );
#endif    
    
    c.erase_if( equal_to_int(1) );
    BOOST_CHECK_EQUAL( c.size(), 5u );
#ifdef PTR_LIST_TEST
    BOOST_CHECK_EQUAL( c.front(), 0 );
#else
    BOOST_CHECK_EQUAL( c[0], 0 );
    BOOST_CHECK_EQUAL( c[1], 2 );
#endif    

    c.erase_if( equal_to_int(7) );
    BOOST_CHECK_EQUAL( c.size(), 4u );
    BOOST_CHECK_EQUAL( c.back(), 6 );

    // C = [0,2,3,6]

    IntContainer c2;
    set_capacity<IntContainer>()( c2 );
    assign::push_back( c2 )
                   ( new int(-1) )
                   ( new int(1) )
                   ( new int(4) )
                   ( new int(5) )
                   ( new int(7) );
    BOOST_CHECK( c2.size() == 5u );
    c.merge( c2 );
    BOOST_CHECK( c2.empty() );
    BOOST_CHECK( c.size() == 9u );
    BOOST_CHECK( is_sorted< std::less_equal<int> >( c ) ); 
    BOOST_TEST_MESSAGE( "finished random accessors algorithms test" );
}

#if defined(BOOST_PTR_CONTAINER_DISABLE_DEPRECATED)
#pragma GCC diagnostic pop
#endif
