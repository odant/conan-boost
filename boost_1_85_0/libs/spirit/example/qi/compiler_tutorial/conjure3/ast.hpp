/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_CONJURE_AST_HPP)
#define BOOST_SPIRIT_CONJURE_AST_HPP

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/support_extended_variant.hpp>
#include <boost/spirit/include/support_attributes.hpp>
#include <boost/optional.hpp>
#include <list>

#include "ids.hpp"

namespace client { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    struct tagged
    {
        int id; // Used to annotate the AST with the iterator position.
                // This id is used as a key to a map<int, Iterator>
                // (not really part of the AST.)
    };

    struct nil {};
    struct unary_expr;
    struct function_call;
    struct expression;

    struct identifier : tagged
    {
        identifier(std::string const& name = "") : name(name) {}
        std::string name;
    };

    struct primary_expr :
        tagged,
        boost::spirit::extended_variant<
            nil
          , bool
          , unsigned int
          , identifier
          , boost::recursive_wrapper<expression>
        >
    {
        primary_expr() : base_type() {}
        primary_expr(bool val) : base_type(val) {}
        primary_expr(unsigned int val) : base_type(val) {}
        primary_expr(identifier const& val) : base_type(val) {}
        primary_expr(expression const& val) : base_type(val) {}
        primary_expr(primary_expr const& rhs)
            : base_type(rhs.get()) {}
    };

    struct operand :
        tagged,
        boost::spirit::extended_variant<
            nil
          , primary_expr
          , boost::recursive_wrapper<unary_expr>
          , boost::recursive_wrapper<function_call>
        >
    {
        operand() : base_type() {}
        operand(primary_expr const& val) : base_type(val) {}
        operand(unary_expr const& val) : base_type(val) {}
        operand(function_call const& val) : base_type(val) {}
        operand(operand const& rhs)
            : base_type(rhs.get()) {}
    };

    struct unary_expr : tagged
    {
        token_ids::type operator_;
        operand operand_;
    };

    struct operation
    {
        token_ids::type operator_;
        operand operand_;
    };

    struct function_call
    {
        identifier function_name;
        std::list<expression> args;
    };

    struct expression
    {
        operand first;
        std::list<operation> rest;
    };

    struct assignment
    {
        identifier lhs;
        token_ids::type operator_;
        expression rhs;
    };

    struct variable_declaration
    {
        identifier lhs;
        boost::optional<expression> rhs;
    };

    struct if_statement;
    struct while_statement;
    struct statement_list;
    struct return_statement;

    typedef boost::variant<
            nil
          , variable_declaration
          , assignment
          , boost::recursive_wrapper<if_statement>
          , boost::recursive_wrapper<while_statement>
          , boost::recursive_wrapper<return_statement>
          , boost::recursive_wrapper<statement_list>
          , boost::recursive_wrapper<expression>
        >
    statement;

    struct statement_list : std::list<statement> {};

    struct if_statement
    {
        expression condition;
        statement then;
        boost::optional<statement> else_;
    };

    struct while_statement
    {
        expression condition;
        statement body;
    };

    struct return_statement : tagged
    {
        boost::optional<expression> expr;
    };

    struct function
    {
        std::string return_type;
        identifier function_name;
        std::list<identifier> args;
        boost::optional<statement_list> body;
    };

    typedef std::list<function> function_list;

    // print functions for debugging
    inline std::ostream& operator<<(std::ostream& out, nil)
    {
        out << "nil"; return out;
    }

    inline std::ostream& operator<<(std::ostream& out, identifier const& id)
    {
        out << id.name; return out;
    }
}}

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::unary_expr,
    (client::token_ids::type, operator_)
    (client::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::operation,
    (client::token_ids::type, operator_)
    (client::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::function_call,
    (client::ast::identifier, function_name)
    (std::list<client::ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::expression,
    (client::ast::operand, first)
    (std::list<client::ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::variable_declaration,
    (client::ast::identifier, lhs)
    (boost::optional<client::ast::expression>, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::assignment,
    (client::ast::identifier, lhs)
    (client::token_ids::type, operator_)
    (client::ast::expression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::if_statement,
    (client::ast::expression, condition)
    (client::ast::statement, then)
    (boost::optional<client::ast::statement>, else_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::while_statement,
    (client::ast::expression, condition)
    (client::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::return_statement,
    (boost::optional<client::ast::expression>, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::function,
    (std::string, return_type)
    (client::ast::identifier, function_name)
    (std::list<client::ast::identifier>, args)
    (boost::optional<client::ast::statement_list>, body)
)

#endif
