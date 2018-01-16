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

// Workaround: No adapt for single member structs
// https://stackoverflow.com/a/19824426/9224499
struct word_t {
  std::string string;

  word_t() = default;
  explicit word_t(std::string str) : string{std::move(str)} {}
};

struct quoted_t {
  std::string string;

  quoted_t() = default;
  explicit quoted_t(std::string str) : string{std::move(str)} {}
};

using value_t = boost::variant<unit_t, double, word_t, quoted_t>;

// Complex types
struct container_t;
struct list_t;
struct pair_t;

using element_t =
    boost::variant<boost::recursive_wrapper<container_t>, list_t, pair_t>;

struct container_t {
  std::string name;
  std::vector<std::string> args;
  std::vector<element_t> elements;
};

struct list_t {
  std::string name;
  std::vector<value_t> values;
};

struct pair_t {
  std::string name;
  value_t value;
};

}  // namespace ast
}  // namespace liberty