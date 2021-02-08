#include <vm.hpp>
#include <iostream>
#include <sstream>
#include <util.hpp>
#include <ux.hpp>
#include "test_fns.hpp"

using namespace std;
using namespace shFT;
using namespace shFT::testing::util;

int main() {

  ux::banner();
  ux::hrwrap("Testing shFT virtual machine.");

  int fail = 0;

  fail += testing::virtual_machine::test_return();
  fail += testing::virtual_machine::test_add();
  fail += testing::virtual_machine::test_sub();
  fail += testing::virtual_machine::test_mul();
  fail += testing::virtual_machine::test_div();

  if (fail) {
    ux::hrwrap("Tests failed!");
  }
  else {
    ux::hrwrap("Tests passed.");
  }

  return fail;
}
