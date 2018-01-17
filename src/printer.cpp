#include "printer.hpp"

namespace liberty {

printer::printer(std::ostream &out) : m_out(out) {}

// Value types
void printer::operator()(const ast::unit_t &unit) {
  m_out << unit.number << unit.unit;
}

void printer::operator()(const double d) { m_out << d; }

void printer::operator()(const ast::word_t &word) { m_out << word.string; }

void printer::operator()(const ast::quoted_t &quoted) {
  m_out << '"' << quoted.string << '"';
}

// Element types
void printer::operator()(const ast::container_t &container) {
  m_out << '\n'
        << std::string(m_tab_size * m_indent_level++, ' ') << container.name
        << '(';
  for (std::size_t i = 0; i < container.args.size(); ++i) {
    if (i != 0) m_out << ", ";
    m_out << container.args[i];
  }
  m_out << ") { \n";

  for (auto &elem : container.elements) boost::apply_visitor(*this, elem);
  m_out << std::string(m_tab_size * --m_indent_level, ' ') << "}\n";
}

void printer::operator()(const ast::list_t &list) {
  m_out << std::string(m_tab_size * m_indent_level, ' ') << list.name << '(';
  for (std::size_t i = 0; i < list.values.size(); ++i) {
    if (i != 0) m_out << ", ";
    boost::apply_visitor(*this, list.values[i]);
  }
  m_out << ");\n";
}

void printer::operator()(const ast::pair_t &pair) {
  m_out << std::string(m_tab_size * m_indent_level, ' ') << pair.name << " : ";
  boost::apply_visitor(*this, pair.value);
  m_out << " ; \n";
}

}  // namespace liberty