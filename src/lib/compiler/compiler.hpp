#pragma once

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <initializer_list>
#include <ast.hpp>
#include <error_handler.hpp>
#include <vm.hpp>
#include "source_function.hpp"

namespace shFT::compiler {
  struct compiler {
    using result_type = bool;

    template <typename ErrorHandler>
    compiler(ErrorHandler& error_handler_) : current(0) {
      using namespace boost::phoenix::arg_names;
      namespace phx = boost::phoenix;
      using boost::phoenix::function;

      error_handler = function<ErrorHandler>(error_handler_)(
          "Error! ", _2, phx::cref(error_handler_.iters)[_1]);
    }

    bool operator()(ast::null) { BOOST_ASSERT(0); return false; }
    bool operator()(unsigned int x);
    bool operator()(bool x);
    bool operator()(ast::identifier const& x);
    bool operator()(ast::operation const& x);
    bool operator()(ast::unary const& x);
    bool operator()(ast::function_call const& x);
    bool operator()(ast::expression const& x);
    bool operator()(ast::assignment const& x);
    bool operator()(ast::variable_declaration const& x);
    bool operator()(ast::statement_list const& x);
    bool operator()(ast::statement const& x);
    bool operator()(ast::if_statement const& x);
    bool operator()(ast::while_statement const& x);
    bool operator()(ast::_return const& x);
    bool operator()(ast::function const& x);
    bool operator()(ast::function_list const& x);

    void print_assembler() const;

    boost::shared_ptr<shFT::compiler::function>
      find_function(std::string const& name) const;

    std::vector<int>& get_code() { return code; }
    std::vector<int> const& get_code() const { return code; }

  private:

    typedef std::map<std::string, boost::shared_ptr<shFT::compiler::function> > function_table;

    std::vector<int> code;
    shFT::compiler::function* current;
    std::string current_function_name;
    function_table functions;
    bool void_return;

    boost::function<
      void(int tag, std::string const& what)>
      error_handler;
  };

}
