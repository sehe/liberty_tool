#pragma once

#include <boost/variant.hpp>
#include <ostream>
#include <string>
#include <vector>

namespace liberty {
namespace ast {

// Value types
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

// Element types
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

// Print functions
inline std::ostream& operator<<(std::ostream& out, const unit_t& unit) {
  out << unit.number << unit.unit;
  return out;
}

inline std::ostream& operator<<(std::ostream& out, const word_t& word) {
  out << word.string;
  return out;
}

inline std::ostream& operator<<(std::ostream& out, const quoted_t& quoted) {
  out << '"' << quoted.string << '"';
  return out;
}

inline std::ostream& operator<<(std::ostream& out,
                                const container_t& container) {
  out << "Container[" << container.name << "(";
  for (std::size_t i = 0; i < container.args.size(); ++i) {
    if (i != 0) out << ", ";
    out << container.args[i];
  }
  out << ")]";
  return out;
}

inline std::ostream& operator<<(std::ostream& out, const list_t& list) {
  out << "List[" << list.name << "(";
  for (std::size_t i = 0; i < list.values.size(); ++i) {
    if (i != 0) out << ", ";
    out << list.values[i];
  }
  out << ")]";
  return out;
}

inline std::ostream& operator<<(std::ostream& out, const pair_t& pair) {
  out << "Pair[" << pair.name << ", " << pair.value << "]";
  return out;
}

}  // namespace ast
}  // namespace liberty