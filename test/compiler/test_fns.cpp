#include "test_fns.hpp"

namespace shFT::testing::compiler {
  std::vector<testcase> testcases() {
    static std::vector<testcase> testcases_ = {
      {"Default Main Function", "int main() { return 0; }", 0},
      {"Main Return Value", "int main() { return 1; }", 1},
      {"Chained Function Call", R"(
int a() { return 1; }
int main() { return a(); }
)", 1},
      {"Recursion", R"(
int callme(a) {
  if (a > 0) {
    return callme(a - 1);
  }
  return 0;
}
int main() {
  return callme(5);
}
)", 0},
      {"Decrement Value", "int main() { int a=5; return a-1; }", 4},
      {"Increment Value", "int main() { int a=5; return a+1; }", 6},
      {"Assignment", "int main() { int a=5; a=3; return a; }", 3},
      {"Boolean True", R"(
int main() {
  if (true) {
    return 0;
  }
  else {
    return 1;
  }
}
)", 0},
      {"Boolean False", R"(
int main() {
  if (false) {
    return 0;
  }
  else {
    return 1;
  }
}
)", 1},
      {"If statement", "int main() { int a=5; if(a>0) { return a; } return 1; }", 5},
      {"If-else statement", R"(
int main() {
  if (false) {
    return 0;
  }
  else {
    return 1;
  }
}
)", 1},
      {"While statement", R"(
int main() {
  int a = 0;
  while (a < 10) {
    a = a + 1;
  }
  return a;
}
)", 10},
    };

    return testcases_;
  }

  std::ostream& operator<<(std::ostream& os, testcase const& tc) {
    os << "Testcase<\n\tname=\"" << tc.name_ << "\",\n\tcode=\"" << tc.code_ 
      << "\",\n\texpected value=" << tc.expect_ << "\n>";
    return os;
  }
}
