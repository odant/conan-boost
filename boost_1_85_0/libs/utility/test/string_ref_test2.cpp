/*
   Copyright (c) Marshall Clow 2012-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <cstddef>
#include <iostream>
#include <cstring>    // for std::strchr
#include <string>
#include <algorithm>

#include <boost/utility/string_ref.hpp>

#include <boost/core/lightweight_test.hpp>

typedef boost::string_ref string_ref;

void ends_with ( const char *arg ) {
    const std::size_t sz = std::strlen ( arg );
    string_ref sr ( arg );
    string_ref sr2 ( arg );
    const char *p = arg;

    while ( *p ) {
        BOOST_TEST ( sr.ends_with ( p ));
        ++p;
        }

    while ( !sr2.empty ()) {
        BOOST_TEST ( sr.ends_with ( sr2 ));
        sr2.remove_prefix (1);
        }

    sr2 = arg;
    while ( !sr2.empty ()) {
        BOOST_TEST ( sr.ends_with ( sr2 ));
        sr2.remove_prefix (1);
        }

    char ch = sz == 0 ? '\0' : arg [ sz - 1 ];
    sr2 = arg;
    if ( sz > 0 )
        BOOST_TEST ( sr2.ends_with ( ch ));
    BOOST_TEST ( !sr2.ends_with ( ++ch ));
    BOOST_TEST ( sr2.ends_with ( string_ref ()));
    }

void starts_with ( const char *arg ) {
    const std::size_t sz = std::strlen ( arg );
    string_ref sr  ( arg );
    string_ref sr2 ( arg );
    std::string foo ( arg );
    while ( !foo.empty ()) {
        BOOST_TEST ( sr.starts_with ( foo ));
        foo.erase ( foo.end () - 1 );
        }

    while ( !sr2.empty ()) {
        BOOST_TEST ( sr.starts_with ( sr2 ));
        sr2.remove_suffix (1);
        }

    char ch = *arg;
    sr2 = arg;
    if ( sz > 0 )
        BOOST_TEST ( sr2.starts_with ( ch ));
    BOOST_TEST ( !sr2.starts_with ( ++ch ));
    BOOST_TEST ( sr2.starts_with ( string_ref ()));
    }

void reverse ( const char *arg ) {
//  Round trip
    string_ref sr1 ( arg );
    std::string string1 ( sr1.rbegin (), sr1.rend ());
    string_ref sr2 ( string1 );
    std::string string2 ( sr2.rbegin (), sr2.rend ());

    BOOST_TEST ( std::equal ( sr2.rbegin (), sr2.rend (), arg ));
    BOOST_TEST ( string2 == arg );
    BOOST_TEST ( std::equal ( sr1.begin (), sr1.end (), string2.begin ()));
    }

// This helper function eliminates signed vs. unsigned warnings
string_ref::size_type ptr_diff ( const char *res, const char *base ) {
    BOOST_TEST ( res >= base );
    return static_cast<string_ref::size_type> ( res - base );
    }

void find ( const char *arg ) {
    string_ref sr1;
    string_ref sr2;
    const char *p;

//  When we search for the empty string, we find it at position 0
    BOOST_TEST ( sr1.find (sr2) == 0 );
    BOOST_TEST ( sr1.rfind(sr2) == 0 );

//  Look for each character in the string(searching from the start)
    p = arg;
    sr1 = arg;
    while ( *p ) {
      string_ref::size_type pos = sr1.find(*p);
      BOOST_TEST ( pos != string_ref::npos && ( pos <= ptr_diff ( p, arg )));
      ++p;
      }

//  Look for each character in the string (searching from the end)
    p = arg;
    sr1 = arg;
    while ( *p ) {
      string_ref::size_type pos = sr1.rfind(*p);
      BOOST_TEST ( pos != string_ref::npos && pos < sr1.size () && ( pos >= ptr_diff ( p, arg )));
      ++p;
      }

// Look for pairs on characters (searching from the start)
    sr1 = arg;
    p = arg;
    while ( *p && *(p+1)) {
        string_ref sr3 ( p, 2 );
        string_ref::size_type pos = sr1.find ( sr3 );
        BOOST_TEST ( pos != string_ref::npos && pos <= static_cast<string_ref::size_type>( p - arg ));
        p++;
        }

    sr1 = arg;
    p = arg;
//  for all possible chars, see if we find them in the right place.
//  Note that strchr will/might do the _wrong_ thing if we search for NULL
    for ( int ch = 1; ch < 256; ++ch ) {
        string_ref::size_type pos = sr1.find(ch);
        const char *strp = std::strchr ( arg, ch );
        BOOST_TEST (( strp == NULL ) == ( pos == string_ref::npos ));
        if ( strp != NULL )
            BOOST_TEST ( ptr_diff ( strp, arg ) == pos );
    }

    sr1 = arg;
    p = arg;
//  for all possible chars, see if we find them in the right place.
//  Note that strchr will/might do the _wrong_ thing if we search for NULL
    for ( int ch = 1; ch < 256; ++ch ) {
        string_ref::size_type pos = sr1.rfind(ch);
        const char *strp = std::strrchr ( arg, ch );
        BOOST_TEST (( strp == NULL ) == ( pos == string_ref::npos ));
        if ( strp != NULL )
            BOOST_TEST ( ptr_diff ( strp, arg ) == pos );
    }


//  Find everything at the start
    p = arg;
    sr1 = arg;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.find(*p);
        BOOST_TEST ( pos == 0 );
        sr1.remove_prefix (1);
        ++p;
        }

//  Find everything at the end
    sr1  = arg;
    p    = arg + std::strlen ( arg ) - 1;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.rfind(*p);
        BOOST_TEST ( pos == sr1.size () - 1 );
        sr1.remove_suffix (1);
        --p;
        }

//  Find everything at the start
    sr1  = arg;
    p    = arg;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.find_first_of(*p);
        BOOST_TEST ( pos == 0 );
        sr1.remove_prefix (1);
        ++p;
        }


//  Find everything at the end
    sr1  = arg;
    p    = arg + std::strlen ( arg ) - 1;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.find_last_of(*p);
        BOOST_TEST ( pos == sr1.size () - 1 );
        sr1.remove_suffix (1);
        --p;
        }

//  Basic sanity checking for "find_first_of / find_first_not_of"
    sr1 = arg;
    sr2 = arg;
    while ( !sr1.empty() ) {
        BOOST_TEST ( sr1.find_first_of ( sr2 )     == 0 );
        BOOST_TEST ( sr1.find_first_not_of ( sr2 ) == string_ref::npos );
        sr1.remove_prefix ( 1 );
        }

    p = arg;
    sr1 = arg;
    while ( *p ) {
        string_ref::size_type pos1 = sr1.find_first_of(*p);
        string_ref::size_type pos2 = sr1.find_first_not_of(*p);
        BOOST_TEST ( pos1 != string_ref::npos && pos1 < sr1.size () && pos1 <= ptr_diff ( p, arg ));
        if ( pos2 != string_ref::npos ) {
            for ( std::size_t i = 0 ; i < pos2; ++i )
                BOOST_TEST ( sr1[i] == *p );
            BOOST_TEST ( sr1 [ pos2 ] != *p );
            }

        BOOST_TEST ( pos2 != pos1 );
        ++p;
        }

//  Basic sanity checking for "find_last_of / find_last_not_of"
    sr1 = arg;
    sr2 = arg;
    while ( !sr1.empty() ) {
        BOOST_TEST ( sr1.find_last_of ( sr2 )     == ( sr1.size () - 1 ));
        BOOST_TEST ( sr1.find_last_not_of ( sr2 ) == string_ref::npos );
        sr1.remove_suffix ( 1 );
        }

    p = arg;
    sr1 = arg;
    while ( *p ) {
        string_ref::size_type pos1 = sr1.find_last_of(*p);
        string_ref::size_type pos2 = sr1.find_last_not_of(*p);
        BOOST_TEST ( pos1 != string_ref::npos && pos1 < sr1.size () && pos1 >= ptr_diff ( p, arg ));
        BOOST_TEST ( pos2 == string_ref::npos || pos1 < sr1.size ());
        if ( pos2 != string_ref::npos ) {
            for ( std::size_t i = sr1.size () -1 ; i > pos2; --i )
                BOOST_TEST ( sr1[i] == *p );
            BOOST_TEST ( sr1 [ pos2 ] != *p );
            }

        BOOST_TEST ( pos2 != pos1 );
        ++p;
        }

    }


void to_string ( const char *arg ) {
    string_ref sr1;
    std::string str1;
    std::string str2;

    str1.assign ( arg );
    sr1 = arg;
// str2 = sr1.to_string<std::allocator<char> > ();
    str2 = sr1.to_string ();
    BOOST_TEST ( str1 == str2 );

#ifndef BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
    std::string str3 = static_cast<std::string> ( sr1 );
    BOOST_TEST ( str1 == str3 );
#endif
    }

void compare ( const char *arg ) {
    string_ref sr1;
    std::string str1;
    std::string str2 = str1;

    str1.assign ( arg );
    sr1 = arg;
    BOOST_TEST ( sr1  == sr1);     // compare string_ref and string_ref
    BOOST_TEST ( sr1  == str1);    // compare string and string_ref
    BOOST_TEST ( str1 == sr1 );    // compare string_ref and string
    BOOST_TEST ( sr1  == arg );    // compare string_ref and pointer
    BOOST_TEST ( arg  == sr1 );    // compare pointer and string_ref

    if ( sr1.size () > 0 ) {
        (*str1.rbegin())++;
        BOOST_TEST ( sr1  != str1 );
        BOOST_TEST ( str1 != sr1 );
        BOOST_TEST ( sr1 < str1 );
        BOOST_TEST ( sr1 <= str1 );
        BOOST_TEST ( str1 > sr1 );
        BOOST_TEST ( str1 >= sr1 );

        (*str1.rbegin()) -= 2;
        BOOST_TEST ( sr1  != str1 );
        BOOST_TEST ( str1 != sr1 );
        BOOST_TEST ( sr1 > str1 );
        BOOST_TEST ( sr1 >= str1 );
        BOOST_TEST ( str1 < sr1 );
        BOOST_TEST ( str1 <= sr1 );
        }
    }

const char *test_strings [] = {
    "",
    "0",
    "abc",
    "AAA",  // all the same
    "adsfadadiaef;alkdg;aljt;j agl;sjrl;tjs;lga;lretj;srg[w349u5209dsfadfasdfasdfadsf",
    "abc\0asdfadsfasf",
    NULL
    };

int main()
{
    const char **p = &test_strings[0];

    while ( *p != NULL ) {
        starts_with ( *p );
        ends_with ( *p );
        reverse ( *p );
        find ( *p );
        to_string ( *p );
        compare ( *p );

        p++;
        }

    return boost::report_errors();
}
