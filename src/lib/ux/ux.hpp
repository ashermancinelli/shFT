
#pragma once
#include <iostream>
#include <sstream>

namespace fortran::ux {
  using std::cout;

  auto hr = [] {
    return std::string(80, '-') + "\n";
  };

  auto hrwrap = [] (std::string msg) {
    std::stringstream ss;
    ss << hr() << msg << "\n" << hr();
    return ss.str();
  };

  auto prompt = [] {
    return "-> ";
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
    return hrwrap(banner_.str());
  };

  auto help = [] {
    static const std::string help_ = R"(
  An empty line parses, compiles, and runs the input.
  Example:

  -> var a = 123;
  -> var b = 456;
  -> var c = a + b * 4;

)";
    return hrwrap(help_);
  };

  auto green = [] (std::string x) {
    return "\033[32m" + x + "\033[39m";
  };

  auto red = [] (std::string x) {
    return "\033[31m" + x + "\033[39m";
  };

  auto reset = [] (std::string x) {
    return "\033[39m" + x;
  };
}
