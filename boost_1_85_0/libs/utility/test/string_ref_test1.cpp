/*
   Copyright (c) Marshall Clow 2012-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <cstddef>
#include <iostream>
#include <algorithm>
#include <string>

#include <boost/utility/string_ref.hpp>

#include <boost/core/lightweight_test.hpp>

typedef boost::string_ref string_ref;

//  Should be equal
void interop ( const std::string &str, string_ref ref ) {
//  BOOST_TEST ( str == ref );
    BOOST_TEST ( str.size () == ref.size ());
    BOOST_TEST ( std::equal ( str.begin (),  str.end (),  ref.begin ()));
    BOOST_TEST ( std::equal ( str.rbegin (), str.rend (), ref.rbegin ()));
    }

void null_tests ( const char *p ) {
//  All zero-length string-refs should be equal
    string_ref sr1; // NULL, 0
    string_ref sr2 ( NULL, 0 );
    string_ref sr3 ( p, 0 );
    string_ref sr4 ( p );
    sr4.clear ();

    BOOST_TEST ( sr1 == sr2 );
    BOOST_TEST ( sr1 == sr3 );
    BOOST_TEST ( sr2 == sr3 );
    BOOST_TEST ( sr1 == sr4 );
    }

//  make sure that substrings work just like strings
void test_substr ( const std::string &str ) {
    const std::size_t sz = str.size ();
    string_ref ref ( str );

//  Substrings at the end
    for ( std::size_t i = 0; i <= sz; ++ i )
        interop ( str.substr ( i ), ref.substr ( i ));

//  Substrings at the beginning
    for ( std::size_t i = 0; i <= sz; ++ i )
        interop ( str.substr ( 0, i ), ref.substr ( 0, i ));

//  All possible substrings
    for ( std::size_t i = 0; i < sz; ++i )
        for ( std::size_t j = i; j < sz; ++j )
            interop ( str.substr ( i, j ), ref.substr ( i, j ));
    }

//  make sure that removing prefixes and suffixes work just like strings
void test_remove ( const std::string &str ) {
    const std::size_t sz = str.size ();
    std::string work;
    string_ref ref;

    for ( std::size_t i = 1; i <= sz; ++i ) {
      work = str;
      ref  = str;
      while ( ref.size () >= i ) {
          interop ( work, ref );
          work.erase ( 0, i );
          ref.remove_prefix (i);
          }
      }

    for ( std::size_t i = 1; i < sz; ++ i ) {
      work = str;
      ref  = str;
      while ( ref.size () >= i ) {
          interop ( work, ref );
          work.erase ( work.size () - i, i );
          ref.remove_suffix (i);
          }
      }
    }

const char *test_strings [] = {
    "",
    "1",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "0123456789",
    NULL
    };

int main()
{
    const char **p = &test_strings[0];

    while ( *p != NULL ) {
        interop ( *p, *p );
        test_substr ( *p );
        test_remove ( *p );
        null_tests ( *p );

        p++;
        }

    return boost::report_errors();
}
