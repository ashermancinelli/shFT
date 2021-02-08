#include "statement_def.hpp"

/*******************************************************************************
 * Instantiation of statement type with the predetermined iterator type.
 ******************************************************************************/
using iterator_type = std::string::const_iterator;
template struct shFT::parser::statement<iterator_type>;
