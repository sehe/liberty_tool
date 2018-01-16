#include "printer.hpp"

namespace liberty {

printer::printer(std::ostream &os) : out(os) {}

// value_t types
void printer::operator()(const ast::unit_t &unit) {
  out << unit.number << unit.unit;
}

void printer::operator()(const double d) { out << d; }

void printer::operator()(const ast::word_t &word) { out << word.str; }

void printer::operator()(const ast::quoted_t &quoted) {
  out << '"' << quoted.str << '"';
}

// element_t types
void printer::operator()(const ast::container &container) {
  out << '\n'
      << std::string(tab_size * indent_level++, ' ') << container.name << '(';
  for (std::size_t i = 0; i < container.args.size(); ++i) {
    if (i != 0) out << ", ";
    out << container.args[i];
  }
  out << ") {\n";
  for (auto &elem : container.elements) boost::apply_visitor(*this, elem);
  out << std::string(tab_size * --indent_level, ' ') << "}\n";
}

void printer::operator()(const ast::list &list) {
  out << std::string(tab_size * indent_level, ' ') << list.name << '(';
  for (std::size_t i = 0; i < list.values.size(); ++i) {
    if (i != 0) out << ", ";
    boost::apply_visitor(*this, list.values[i]);
  }
  out << ");\n";
}

void printer::operator()(const ast::pair &pair) {
  out << std::string(tab_size * indent_level, ' ') << pair.name << " : ";
  boost::apply_visitor(*this, pair.value);
  out << " ;\n";
}

}  // namespace liberty