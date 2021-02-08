#pragma once
#include "ast.hpp"
#include "expression.hpp"
#include "annotation.hpp"
#include "error_handler.hpp"

namespace fortran::parser {
  template <typename Iterator>
  expression<Iterator>::expression(error_handler<Iterator>& error_handler)
    : expression::base_type(expr) {
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    qi::char_type char_;
    qi::uint_type uint_;
    qi::_val_type _val;
    qi::raw_type raw;
    qi::lexeme_type lexeme;
    qi::alpha_type alpha;
    qi::alnum_type alnum;
    qi::bool_type bool_;

    using qi::on_error;
    using qi::on_success;
    using qi::fail;
    using boost::phoenix::function;

    using error_handler_function = function<::fortran::error_handler<Iterator>>;
    using annotation_function = function<::fortran::annotation<Iterator>>;

    /***************************************************************************
     * Tokens
     **************************************************************************/
    logical_or_op.add
      ("||", ast::or_)
      ;

    logical_and_op.add
      ("&&", ast::and_)
      ;

    equality_op.add
      ("==", ast::equal_)
      ("!=", ast::not_equal_)
      ;

    relational_op.add
      ("<", ast::less_)
      ("<=", ast::less_equal_)
      (">", ast::greater_)
      (">=", ast::greater_equal_)
      ;

    additive_op.add
      ("+", ast::plus_)
      ("-", ast::minus_)
      ;

    multiplicative_op.add
      ("*", ast::times_)
      ("/", ast::divide_)
      ;

    unary_op.add
      ("+", ast::positive_)
      ("-", ast::negative_)
      ("!", ast::not_)
      ;

    keywords.add
      ("true")
      ("false")
      ("if")
      ("else")
      ("while")
      ("int")
      ("void")
      ("return")
      ;

    /***************************************************************************
     * Main expression grammar
     **************************************************************************/
    expr =
      logical_or_expr.alias()
      ;

    logical_or_expr =
      logical_and_expr
      >> *(logical_or_op > logical_and_expr)
      ;

    logical_and_expr =
      equality_expr
      >> *(logical_and_op > equality_expr)
      ;

    equality_expr =
      relational_expr
      >> *(equality_op > relational_expr)
      ;

    relational_expr =
      additive_expr
      >> *(relational_op > additive_expr)
      ;

    additive_expr =
      multiplicative_expr
      >> *(additive_op > multiplicative_expr)
      ;

    multiplicative_expr =
      unary_expr
      >> *(multiplicative_op > unary_expr)
      ;

    unary_expr =
      primary_expr
      |   (unary_op > unary_expr)
      ;

    primary_expr =
      uint_
      |   function_call
      |   identifier
      |   bool_
      |   '(' > expr > ')'
      ;

    function_call =
      (identifier >> '(')
      >   argument_list
      >   ')'
      ;

    argument_list = -(expr % ',');

    identifier =
      !lexeme[keywords >> !(alnum | '_')]
      >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
      ;

    /***************************************************************************
     * Debugging and error handling and reporting support.
     **************************************************************************/
    BOOST_SPIRIT_DEBUG_NODES(
        (expr)
        (logical_or_expr)
        (logical_and_expr)
        (equality_expr)
        (relational_expr)
        (additive_expr)
        (multiplicative_expr)
        (unary_expr)
        (primary_expr)
        // (function_call)
        (argument_list)
        (identifier)
        );

    /***************************************************************************
     * Error handling: on error in expr, call error_handler.
     **************************************************************************/
    on_error<fail>(expr,
        error_handler_function(error_handler)(
          "Error! Expecting ", _4, _3));

    /***************************************************************************
     * Annotation: on success in primary_expr, call annotation.
     **************************************************************************/
    on_success(primary_expr,
        annotation_function(error_handler.iters)(_val, _1));
  }
}
