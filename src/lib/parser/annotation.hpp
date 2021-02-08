#pragma once

#include <map>
#include <boost/variant/apply_visitor.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>
#include "ast.hpp"

/*******************************************************************************
 * Annotator for AST elements.
 *
 * The annotation handler links the AST to a map of iterator positions for the
 * purpose of subsequent semantic error handling when the program is being
 * compiled.
 ******************************************************************************/
namespace fortran {
  template <typename Iterator>
  struct annotation {
    template <typename, typename>
    struct result {
      using type = void;
    };

    std::vector<Iterator>& iters;
    annotation(std::vector<Iterator>& iters)
      : iters(iters) {}

    struct set_id {
      using result_type = void;

      int id;
      set_id(int id) : id(id) {}

      /*
      void operator()(ast::function_call& x) const {
        x.function_name.id = id;
      }
      */

      void operator()(ast::identifier& x) const {
        x.id = id;
      }

      template <typename T>
      void operator()(T& x) const {}
    };

    void operator()(ast::operand& ast, Iterator pos) const {
      int id = iters.size();
      iters.push_back(pos);
      boost::apply_visitor(set_id(id), ast);
    }

    void operator()(ast::variable_declaration& ast, Iterator pos) const {
      int id = iters.size();
      iters.push_back(pos);
      ast.lhs_.id = id;
    }

    void operator()(ast::assignment& ast, Iterator pos) const {
      int id = iters.size();
      iters.push_back(pos);
      ast.lhs_.id = id;
    }

    void operator()(ast::_return& ast, Iterator pos) const {
      int id = iters.size();
      iters.push_back(pos);
      ast.id = id;
    }

    void operator()(ast::identifier& ast, Iterator pos) const {
      int id = iters.size();
      iters.push_back(pos);
      ast.id = id;
    }
  };

}
