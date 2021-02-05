#pragma once
#include <iostream>

namespace fortran::testing::util {

  using namespace std;

  constexpr int one=1, two=2, three=3, four=4, six=6;

  constexpr auto return_ = [](int fail, std::string func) {
    cout << "TEST::" << func << "::" << (fail ? "FAIL" : "PASS") << "\n";
    return fail;
  };

  constexpr auto line = [] {
    for (int i=0; i<80; i++)
      cout << '-';
    cout << "\n";
  };
}
