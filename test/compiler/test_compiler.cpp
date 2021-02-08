#include <ast.hpp>
#include <vm.hpp>
#include <ux.hpp>
#include <compiler.hpp>
#include <parser.hpp>
#include <error_handler.hpp>
#include <config.hpp>
#include <iostream>
#include <functional>
#include "test_fns.hpp"

using namespace std;
using namespace fortran;
using namespace fortran::testing::compiler;

int main() {

  ux::banner();
  ux::hrwrap("Testing fortran compiler and runtime.");

  int fail = 0;
  auto tcs = ::fortran::testing::compiler::testcases();
  for (auto const& testcase_ : tcs) {
    using fortran::parser::iterator_type;
    iterator_type it = testcase_.code_.begin(), end = testcase_.code_.end();

    fortran::virtual_machine vm;
    fortran::compiler::program prog;
    fortran::ast::statement_list ast;

    using fortran::parser::error_handler_type;
    using boost::spirit::x3::with;
    using boost::spirit::x3::ascii::space;
    error_handler_type error_handler(it, end, std::cerr);
    fortran::compiler::compiler compile(prog, error_handler);
    auto const parser =
      with<fortran::parser::error_handler_tag>(std::ref(error_handler)) [
        fortran::get_parser()
      ];

    bool r = false;
    try {
      r = phrase_parse(it, end, parser, space, ast);
    }
    catch (std::exception& e) {
      cout << "Failed testcase: "<< testcase_ << " with exception:\n"
        << e.what() << "\n";
      fail++;
      continue;
    }

    if (!r || it != end) {
      cout << "Failed testcase: "<< testcase_ << " due to incomplete parse.\n";
      fail++;
      continue;
    }

    r = compile.start(ast);
    if (!r) {
      cout << "Failed testcase: " << testcase_ << " due to failed compilation.\n";
      fail++;
      continue;
    }

    int ret = vm.execute(prog());
    if (ret != testcase_.expect_) {
      cout << "Failed testcase: " << testcase_ << ". Actual value="
        << ret << "\n";
      cout << "Assembly:\n";
      prog.print_assembler();
      cout << "Variables:\n";
      prog.print_variables(vm.get_stack());
      fail++;
      continue;
    }

    cout << "PASS: " << testcase_ << "\n";
  }

  if (fail) {
    std::stringstream ss;
    ss << "Found " << fail << " failure(s).";
    ux::hrwrap(ss.str());
  }
  else {
    ux::hrwrap("Tests passed.");
  }

  return fail;
}
