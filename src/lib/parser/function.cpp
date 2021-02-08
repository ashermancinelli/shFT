#include "function_def.hpp"

/*******************************************************************************
 * Function Rule Definitions
 *
 * Instantiation of template specializations of types declared in `function.hpp`
 * and `function_def.hpp`.
 ******************************************************************************/
typedef std::string::const_iterator iterator_type;
template struct shFT::parser::function<iterator_type>;
