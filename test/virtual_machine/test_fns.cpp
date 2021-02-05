#include "test_fns.hpp"
#include <vector>
#include <iostream>

using namespace std;

namespace fortran::testing::virtual_machine {

  using namespace ::fortran::testing::util;

  int test_return() {
    int fail=0;
    const int expect = two;

    vector<int> code = {
      /* Push 5 onto the stack */
      op_int, two,
      /* Return with exit code 5 */
      op_return,
    };

    fortran::virtual_machine vm;
    int ret = vm.execute(code);
    if (ret != expect)
      fail++;

    return return_(fail, __func__);
  }

  int test_add() {
    int fail=0;
    const int expect = four;

    vector<int> code = {
      op_int, two,
      op_int, two,
      op_add,
      op_return,
    };

    fortran::virtual_machine vm;
    int ret = vm.execute(code);
    if (ret != expect)
      fail++;

    return return_(fail, __func__);
  }

  int test_sub() {
    int fail = 0;
    const int expect = 0;

    vector<int> code = {
      op_int, two,
      op_int, two,
      op_sub,
      op_return,
    };

    fortran::virtual_machine vm;
    int ret = vm.execute(code);

    if (ret != expect)
      fail++;

    return return_(fail, __func__);
  }

  int test_mul() {
    int fail = 0;
    const int expect = four;

    vector<int> code = {
      op_int, two,
      op_int, two,
      op_mul,
      op_return,
    };

    fortran::virtual_machine vm;
    int ret = vm.execute(code);

    if (ret != expect)
      fail++;

    return return_(fail, __func__);
  }

  int test_div() {
    int fail = 0;
    const int expect = one;

    vector<int> code = {
      op_int, two,
      op_int, two,
      op_div,
      op_return,
    };

    fortran::virtual_machine vm;
    int ret = vm.execute(code);

    if (ret != expect)
      fail++;

    return return_(fail, __func__);
  }
}
