#pragma once

#include <boost/variant.hpp>
#include <string>
#include <vector>

namespace liberty {
namespace ast {

// Simple types
struct unit_t {
  double number;
  std::string unit;
};

// Workaround: No adapt for single member struct
// https://stackoverflow.com/a/19824426/9224499
struct word_t {
  std::string str;

  word_t() = default;
  explicit word_t(std::string s) : str(std::move(s)) {}
};

struct quoted_t {
  std::string str;

  quoted_t() = default;
  explicit quoted_t(std::string s) : str(std::move(s)) {}
};

using value_t = boost::variant<unit_t, double, word_t, quoted_t>;

// Complex types
struct container;
struct list;
struct pair;

using element_t =
    boost::variant<boost::recursive_wrapper<container>, list, pair>;

struct container {
  std::string name;
  std::vector<std::string> args;
  std::vector<element_t> elements;
};

struct list {
  std::string name;
  std::vector<value_t> values;
};

struct pair {
  std::string name;
  value_t value;
};

}  // namespace ast
}  // namespace liberty