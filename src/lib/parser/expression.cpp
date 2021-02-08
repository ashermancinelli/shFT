#include "expression_def.hpp"

/*******************************************************************************
 * Instantiation of expression type with the predetermined iterator type.
 ******************************************************************************/
using iterator_type = std::string::const_iterator;
template struct shFT::parser::expression<iterator_type>;
