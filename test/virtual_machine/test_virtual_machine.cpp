#include <vm.hpp>
#include <iostream>
#include <util.hpp>
#include "test_fns.hpp"

using namespace std;
using namespace fortran;
using namespace fortran::testing::util;

int main() {

  line();
  cout << "Testing fortran virtual machine.\n";
  line();

  int fail = 0;

  fail += testing::test_return();
  fail += testing::test_add();
  fail += testing::test_sub();
  fail += testing::test_mul();
  fail += testing::test_div();

  line();
  if (fail) {
    cout << "Tests failed!\n";
  }
  else {
    cout << "Tests Passed.\n";
  }
  line();

  return fail;
}
