#include "test_fns.hpp"

namespace fortran::testing::compiler {
  std::vector<testcase> testcases() {
    static std::vector<testcase> testcases_ = {
      {"Declaration", "integer a = 5; return a;", 5},
      {"Add", R"(
integer a = 5;
a = a + 3;
return a;
)", 8},
      {"If statement", R"(
integer a = 5;
if a > 3 {
  return true;
}
else {
  return false;
}
)", 1},
    };

    return testcases_;
  }

  std::ostream& operator<<(std::ostream& os, testcase const& tc) {
    os << "Testcase<\n\tname=\"" << tc.name_ << "\",\n\tcode=\"" << tc.code_ 
      << "\",\n\texpected value=" << tc.expect_ << "\n>";
    return os;
  }
}
