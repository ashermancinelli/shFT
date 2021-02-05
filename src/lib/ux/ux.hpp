
#pragma once
#include <iostream>

namespace fortran::ux {
  using std::cout;

  auto hr = [] {
    for(int i=0; i<80; i++)
      cout << '-';
    cout << "\n";
  };

  auto hrwrap = [] (std::string msg) {
    hr();
    cout << msg << "\n";
    hr();
  };

  auto prompt = [] {
    cout << "-> ";
  };

  auto banner = [] {
    static std::stringstream banner_;
    banner_ << R"(
               /        //   / / /__  ___/
      ___     //__     //___       / /
    ((   ) ) //   ) ) / ___       / /
     \ \    //   / / //          / /
  //   ) ) //   / / //          / /

  shFT: Parser, Compiler, and Virtual Machine.
  Compiled on )" << __DATE__ << R"(
  Type 'help' for more information.
)";
    hrwrap(banner_.str());
  };

  auto help = [] {
    static const std::string help_ = R"(
  An empty line parses, compiles, and runs the input.
  Example:

  -> var a = 123;
  -> var b = 456;
  -> var c = a + b * 4;

)";
    hrwrap(help_);
  };
}
