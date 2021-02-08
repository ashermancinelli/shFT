#pragma once
#include <ast.hpp>
#include <vm.hpp>
#include <ux.hpp>
#include <compiler.hpp>
#include <statement.hpp>
#include <error_handler.hpp>
#include <string>
#include <tuple>
#include <array>
#include <iostream>

namespace fortran::testing::compiler {
  /* Each test case contains code to compile and run in the virtual machine
   * as well as the expected output on the top of the stack */
  struct testcase {
    std::string name_;
    std::string code_;
    int expect_;
  };
  std::ostream& operator<<(std::ostream& os, testcase const& tc);
  std::vector<testcase> testcases();
}

