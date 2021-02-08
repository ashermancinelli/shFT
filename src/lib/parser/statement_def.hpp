#pragma once

#include "statement.hpp"
#include "error_handler.hpp"
#include "annotation.hpp"

namespace shFT::parser {
  /*****************************************************************************
   * Implementation of Statement Grammar.
   *
   * @see "statement.hpp" for declarations.
   ****************************************************************************/

  /* Wrap string in a lexeme and ensure it's not followed by another illegal
   * char */
  auto mkkw = [] (std::string const& x) {
    return lexeme[x >> !(alnum | '_')];
  };

  template <typename Iterator>
  statement<Iterator>::statement(error_handler<Iterator>& error_handler) :
    statement::base_type(statement_list), expr(error_handler) {
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    qi::_val_type _val;
    qi::raw_type raw;
    qi::lexeme_type lexeme;
    qi::alpha_type alpha;
    qi::alnum_type alnum;
    qi::lit_type lit;

    using qi::on_error;
    using qi::on_success;
    using qi::fail;
    using boost::phoenix::function;

    using error_handler_function = function<::shFT::error_handler<Iterator>>;
    using annotation_function = function<::shFT::annotation<Iterator>>;

    statement_list =
      +statement_
      ;

    statement_ =
      variable_declaration
      |   assignment
      |   compound_statement
      |   if_statement
      |   while_statement
      |   _return
      ;

    identifier =
      !expr.keywords
      >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
      ;

    variable_declaration =
      lexeme["int" >> !(alnum | '_')] // make sure we have whole words
      >   identifier
      >   -('=' > expr)
      >   ';'
      ;

    assignment =
      identifier
      >   '='
      >   expr
      >   ';'
      ;

    if_statement =
      lit("if")
      >   '('
      >   expr
      >   ')'
      >   statement_
      >
      -(
          lexeme["else" >> !(alnum | '_')] // make sure we have whole words
          >   statement_
       )
      ;

    while_statement =
      lit("while")
      >   '('
      >   expr
      >   ')'
      >   statement_
      ;

    compound_statement =
      '{' >> -statement_list >> '}'
      ;

    _return =
      lexeme["return" >> !(alnum | '_')] // make sure we have whole words
      >  -expr
      >   ';'
      ;

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODES(
        (statement_list)
        (identifier)
        (variable_declaration)
        (assignment)
        );

    // Error handling: on error in statement_list, call error_handler.
    on_error<fail>(statement_list,
        error_handler_function(error_handler)(
          "Error! Expecting ", _4, _3));

    // Annotation: on success in variable_declaration,
    // assignment and return_statement, call annotation.
    on_success(variable_declaration,
        annotation_function(error_handler.iters)(_val, _1));
    on_success(assignment,
        annotation_function(error_handler.iters)(_val, _1));
    on_success(_return,
        annotation_function(error_handler.iters)(_val, _1));
  }

}
