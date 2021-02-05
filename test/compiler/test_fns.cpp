#include "test_fns.hpp"

namespace fortran::testing::compiler {
  std::vector<testcase> testcases() {
    static std::vector<testcase> testcases_ = {
      {"Declaration", "var a = 5;", 5},
    };

    return testcases_;
  }

  std::ostream& operator<<(std::ostream& os, testcase const& tc) {
    os << "Testcase<\n\tname=\"" << tc.name_ << "\",\n\tcode=\"" << tc.code_ 
      << "\",\n\texpected value=" << tc.expect_ << "\n>";
    return os;
  }
}
