#pragma once
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include "ast_adapt.hpp"
#include "symbols.hpp"
#include "config.hpp"

/*******************************************************************************
 * `parser.hpp`
 *
 * This header is broken up into four sections:
 *
 * - Helper functions
 * - Expression declarations
 * - Expression definitions
 * - Statement declarations
 * - Statement definitions
 *
 * In the boost documentation, it is often reccommended to break up the parser
 * logic into multiple translation units, however this requires heavy reliance
 * on macros such as BOOST_SPIRIT_DECLARE, BOOST_SPIRIT_DEFINE, and
 * BOOST_SPIRIT_INSTANTIATE, organization of .hpp, _def.hpp, and .cpp files,
 * and strict management of which headers are included in which sources[1].
 * Stack overflow user sehe is extremenly active on stack overflow, watching
 * tags related to boost and spirit in particular, and they reccommend *never*
 * breaking up the parsing logic into multiple TUs[2].
 *
 * The alternative to using spirit would likely be flex/bison, in which case we
 * would need all the parser logic in one file anyways, so I don't think the
 * cognitive burden of having a header this large is too great.
 *
 * - [1]: https://www.boost.org/doc/libs/1_75_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/minimal.html
 * - [2]: https://stackoverflow.com/questions/66036568/mixing-non-terminal-rules-from-separeted-translation-unit
 *
 ******************************************************************************/

namespace fortran::parser {
  using x3::raw;
  using x3::lexeme;
  using x3::rule;
  using x3::alpha;
  using x3::alnum;
  namespace ascii = x3::ascii;

  /*****************************************************************************
   * Helper Functions
   ****************************************************************************/

  /* Return lexeme not followed by a character */
  auto const mkkw = [](auto x) {
    return lexeme[ x >> !(alnum | '_') ];
  };

  /*****************************************************************************
   * Declaration and definition of identifier rules
   ****************************************************************************/
  x3::rule<struct identifier, std::string> const identifier = "identifier";

  auto const identifier_def = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

  BOOST_SPIRIT_DEFINE(identifier);

  /*****************************************************************************
   * Declaration of expression rules
   ****************************************************************************/
  using expression_type = x3::rule<struct expression, ast::expression>;
  expression_type const expression = "expression";

  rule<struct equality_expression, ast::expression>
    const equality_expression = "equality_expression";

  rule<struct relational_expression, ast::expression>
    const relational_expression = "relational_expression";

  rule<struct logical_expression, ast::expression>
    const logical_expression = "logical_expression";

  rule<struct additive_expression, ast::expression>
    const additive_expression = "additive_expression";

  rule<struct multiplicative_expression, ast::expression>
    const multiplicative_expression = "multiplicative_expression";

  rule<struct unary_expression, ast::operand>
    const unary_expression = "unary_expression";

  rule<struct primary_expression, ast::operand>
    const primary_expression = "primary_expression";

  /*****************************************************************************
   * Definition of expression rules
   ****************************************************************************/
  auto const logical_expression_def =
    equality_expression
    >> *(logical_op > equality_expression)
    ;

  auto const equality_expression_def =
    relational_expression
    >> *(equality_op > relational_expression)
    ;

  auto const relational_expression_def =
    additive_expression
    >> *(relational_op > additive_expression)
    ;

  auto const additive_expression_def =
    multiplicative_expression
    >> *(additive_op > multiplicative_expression)
    ;

  auto const multiplicative_expression_def =
    unary_expression
    >> *(multiplicative_op > unary_expression)
    ;

  auto const unary_expression_def =
    primary_expression
    |   (unary_op > primary_expression)
    ;

  auto const primary_expression_def =
    uint_
    |   bool_
    |   (!keywords >> identifier)
    |   '(' > expression > ')'
    ;

  auto const expression_def = logical_expression;

  BOOST_SPIRIT_DEFINE(
    expression
    , logical_expression
    , equality_expression
    , relational_expression
    , additive_expression
    , multiplicative_expression
    , unary_expression
    , primary_expression
  );

  struct unary_expression : x3::annotate_on_success {};
  struct primary_expression : x3::annotate_on_success {};

  /*****************************************************************************
   * Declaration of statement rules
   ****************************************************************************/
  using statement_type = rule<struct statement, ast::statement>;
  statement_type statement = "statement";

  using statement_list_type = rule<struct statement_list, ast::statement_list>;
  statement_list_type statement_list = "statement_list";

  rule<struct variable_declaration, ast::variable_declaration>
    variable_declaration = "variable_declaration";

  rule<struct assignment, ast::assignment>
    assignment = "assignment";

  rule<struct variable, ast::variable>
    variable = "variable";

  rule<struct _return, ast::_return>
    _return = "_return";

  rule<struct if_statement, ast::if_statement>
    if_statement = "if_statement";

  rule<struct block, ast::block>
    block = "block";

  /*****************************************************************************
   * Definitions of statement rules
   ****************************************************************************/

  /* Placeholder for individual statements. We don't want this to conflict with
   * the definition of the actual statement AST node, but we also want to be
   * able to refer to a single statement */
  auto const statement_list_def =
    +(
       _return
       | if_statement 
       | variable_declaration
       | assignment
    )
    ;

  auto const block_def =
    char_('{')
    >   statement
    >   char_('}')
    ;

  auto const if_statement_def =
    mkkw("if")
    >   '(' > expression > ')'
    >   '{' > statement > '}'
    >   -(mkkw("else") > '{' > statement > '}')
    ;

  auto const _return_def =
    lexeme["return" >> !(alnum | '_')]
    >   expression
    >   ';'
    ;

  auto const variable_declaration_def =
    lexeme["integer" >> !(alnum | '_')] // make sure we have whole words
    >   assignment
    ;

  auto const assignment_def =
    variable
    >   '='
    >   expression
    >   ';'
    ;

  auto const variable_def = identifier;
  auto const statement_def = statement_list;

  BOOST_SPIRIT_DEFINE(
    statement
    , statement_list
    , variable_declaration
    , assignment
    , variable
    , _return
    , if_statement
    , block
  );

  struct statement : error_handler_base, x3::annotate_on_success {};
  struct assignment : x3::annotate_on_success {};
  struct variable : x3::annotate_on_success {};
}

namespace fortran {
  inline ::fortran::parser::statement_type const& get_parser() {
    ::fortran::parser::add_keywords();
    return ::fortran::parser::statement;
  }
}
