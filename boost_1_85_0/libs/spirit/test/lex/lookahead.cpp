//  Copyright (c) 2001-2011 Hartmut Kaiser
//  Copyright (c) 2010 Mathias Gaunard
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_SPIRIT_DEBUG 1    // required for token streaming
// #define BOOST_SPIRIT_LEXERTL_DEBUG 1

#include <boost/spirit/include/lex_lexertl.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/statement.hpp>

#include <sstream>

namespace spirit = boost::spirit;
namespace lex = spirit::lex;
namespace phoenix = boost::phoenix;

typedef char const* content_iterator;

struct string_literal
{
    string_literal(content_iterator, content_iterator)
    {
    }
};

typedef lex::lexertl::token<
    content_iterator, boost::mpl::vector<string_literal>
> token_type;

struct lexer
  : lex::lexer<lex::lexertl::actor_lexer<token_type> >
{
    lexer() : st("'[^'\\n]*'", 1)
    {
        lex::token_def<> string_lookahead('\'');
        self("LA") = string_lookahead;

        // make sure lookahead is implicitly evaluated using the lexer state
        // the token_def has been associated with
        self = st [
                phoenix::if_(lex::lookahead(string_lookahead)) [ lex::more() ]
            ]
            ;
    }
    
    lex::token_def<string_literal> st;
};

typedef lexer::iterator_type token_iterator;

int main()
{
    std::string const s = "'foo''bar'";

    content_iterator begin = s.data();
    content_iterator end = s.data() + s.size();

    lexer l;
    token_iterator begin2 = l.begin(begin, end);
    token_iterator end2 = l.end();

    char const* test_data[] = { "1,'foo'", "1,'foo''bar'" };
    std::size_t const test_data_size = sizeof(test_data)/sizeof(test_data[0]);

    token_iterator it = begin2;
    std::size_t i = 0;
    for (/**/; it != end2 && i < test_data_size; ++it, ++i)
    {
        std::stringstream ss2;
        ss2 << it->id() << "," << *it;
        BOOST_TEST(ss2.str() == test_data[i]);
    }
    BOOST_TEST(it == end2);
    BOOST_TEST(i == test_data_size);

    return boost::report_errors();
}
