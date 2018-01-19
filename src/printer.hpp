#pragma once

#include <boost/variant.hpp>
#include <ostream>
#include "ast_types.hpp"

namespace liberty {

// Prints liberty file similar to SiliconSmart
class printer : public boost::static_visitor<void> {
 public:
  printer(std::ostream &out);

  // Value types
  void operator()(const ast::unit_t &unit);
  void operator()(double number);
  void operator()(const ast::word_t &word);
  void operator()(const ast::quoted_t &quoted);

  // Element types
  void operator()(const ast::container_t &container);
  void operator()(const ast::list_t &list);
  void operator()(const ast::pair_t &pair);

 private:
  std::ostream &m_out;
  int m_indent_level = 0;
  const int m_tab_size = 2;
};

}  // namespace liberty