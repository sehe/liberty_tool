#pragma once

#include <boost/variant.hpp>
#include <ostream>
#include "ast_types.hpp"

namespace liberty {

// Prints liberty file similar to SiliconSmart
class printer : public boost::static_visitor<void> {
 public:
  printer(std::ostream &os);

  // value_t types
  void operator()(const ast::unit_t &unit);
  void operator()(const double d);
  void operator()(const ast::word_t &word);
  void operator()(const ast::quoted_t &quoted);

  // element_t types
  void operator()(const ast::container &container);
  void operator()(const ast::list &list);
  void operator()(const ast::pair &pair);

 private:
  std::ostream &out;
  int indent_level = 0;
  const int tab_size = 2;
};

}  // namespace liberty