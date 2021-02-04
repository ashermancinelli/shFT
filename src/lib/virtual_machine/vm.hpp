#pragma once
#include <vector>

namespace fortran {
  enum byte_code
  {
    op_neg,         //  negate the top stack entry
    op_add,         //  add top two stack entries
    op_sub,         //  subtract top two stack entries
    op_mul,         //  multiply top two stack entries
    op_div,         //  divide top two stack entries

    op_not,         //  boolean negate the top stack entry
    op_eq,          //  compare the top two stack entries for ==
    op_neq,         //  compare the top two stack entries for !=
    op_lt,          //  compare the top two stack entries for <
    op_lte,         //  compare the top two stack entries for <=
    op_gt,          //  compare the top two stack entries for >
    op_gte,         //  compare the top two stack entries for >=

    op_and,         //  logical and top two stack entries
    op_or,          //  logical or top two stack entries

    op_load,        //  load a variable
    op_store,       //  store a variable

    op_int,         //  push constant integer into the stack
    op_true,        //  push constant 0 into the stack
    op_false,       //  push constant 1 into the stack

    op_jump_if,     //  jump to a relative position in the code if top stack
                    //  evaluates to false
    op_jump,        //  jump to a relative position in the code

    op_stk_adj,     // adjust the stack (for args and locals)
    op_call,        // function call
    op_return       // return from function
  };

  struct virtual_machine {
    virtual_machine(std::size_t stack_size=4096) : stack_(stack_size) {}

    /* Execute instructions from code segment.
     * This is used to recursively call functions */
    int execute(std::vector<int> const& code,
        std::vector<int>::const_iterator pc,
        std::vector<int>::iterator frame_ptr);

    /* Main entry point for executing code. */
    int execute(std::vector<int> const& code) {
      return execute(code, code.begin(), stack_.begin());
    }

  private:
    std::vector<int> stack_;
  };
}
