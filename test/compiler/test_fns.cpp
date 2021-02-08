#include "test_fns.hpp"

namespace fortran::testing::compiler {
  std::vector<testcase> testcases() {
    static std::vector<testcase> testcases_ = {
      {"Default Main Function", "int main() { return 0; }", 0},
      {"Main Return Value", "int main() { return 1; }", 1},
      {"Chained Function Call", R"(
int a() { return 1; }
int main() { return a(); }
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
