#pragma once

#include "ast.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::unary,
    operator_, operand_);

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::operation,
    operator_, operand_);

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::expression,
    first_, rest_);

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::variable_declaration,
    assignment_);

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::assignment,
    lhs_, rhs_);

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::if_statement,
    condition_, body_, else_);

BOOST_FUSION_ADAPT_STRUCT(fortran::ast::while_statement,
    condition_, body_);
