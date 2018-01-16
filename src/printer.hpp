#pragma once

#include <boost/variant.hpp>
#include <ostream>
#include "ast_types.hpp"

namespace liberty {

// Prints liberty file similar to SiliconSmart
class printer : public boost::static_visitor<void> {
 public:
  printer(std::ostream &os);

  // value types
  void operator()(const ast::unit_t &unit);
  void operator()(const double d);
  void operator()(const ast::word_t &word);
  void operator()(const ast::quoted_t &quoted);

  // element types
  void operator()(const ast::container_t &container);
  void operator()(const ast::list_t &list);
  void operator()(const ast::pair_t &pair);

 private:
  std::ostream &out;
  int indent_level = 0;
  const int tab_size = 2;
};

}  // namespace liberty