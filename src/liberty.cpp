#include "liberty.hpp"

#include <algorithm>
#include <boost/variant.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include "liberty_grammar.hpp"
#include "printer.hpp"
#include "skipper_grammar.hpp"

namespace liberty {
namespace {

// Convert value to string for regex_matcher.
class stringer : public boost::static_visitor<std::string> {
 public:
  // Value types
  std::string operator()(const ast::unit_t &unit) const {
    return std::to_string(unit.number) + unit.unit;
  }

  std::string operator()(const double d) const { return std::to_string(d); }

  std::string operator()(const ast::word_t &word) const { return word.string; }

  std::string operator()(const ast::quoted_t &quoted) const {
    return quoted.string;  // no quotes added!
  }
};

// Checks if element matches given regex.
class regex_matcher : public boost::static_visitor<bool> {
 public:
  regex_matcher(std::regex pattern) : m_pattern{std::move(pattern)} {}

  // Element types
  bool operator()(const ast::container_t &container) const {
    std::vector<std::string> targets{container.name};
    for (std::size_t i = 0; i < container.args.size(); ++i)
      targets.push_back(container.name + ':' +
                        boost::apply_visitor(stringer{}, container.args[i]));

    return std::any_of(targets.begin(), targets.end(),
                       [&](const std::string &target) {
                         return std::regex_match(target, m_pattern);
                       });
  }

  bool operator()(const ast::list_t &list) const {
    return std::regex_match(list.name, m_pattern);  // add values as well?
  }

  bool operator()(const ast::pair_t &pair) const {
    std::vector<std::string> targets{
        pair.name,
        pair.name + ':' + boost::apply_visitor(stringer{}, pair.value)};

    return std::any_of(targets.begin(), targets.end(),
                       [&](const std::string &target) {
                         return std::regex_match(target, m_pattern);
                       });
  }

 private:
  std::regex m_pattern;
};

// Generates back references for walking up the tree.
void generate_parents(std::map<ast::element_t *, ast::element_t *> &parents,
                      ast::element_t &element) {
  if (auto container_ptr = boost::get<ast::container_t>(&element)) {
    for (auto &child : container_ptr->elements) {
      parents.emplace(&child, &element);
      generate_parents(parents, child);
    }
  }
}

}  // namespace

node::node(std::vector<ast::element_t *> elements, const liberty &lib)
    : m_elements(std::move(elements)), m_lib(&lib) {}

node node::operator[](const std::regex &pattern) {
  // Get children from all container nodes
  // (Other node types don't have children)
  std::vector<ast::element_t *> children;

  for (auto elem_ptr : m_elements) {
    if (auto cont_ptr = boost::get<ast::container_t>(elem_ptr)) {
      for (auto &elem : cont_ptr->elements) children.push_back(&elem);
    }
  }

  // Filter gathered children by regex
  regex_matcher matcher{pattern};
  std::vector<ast::element_t *> matched;

  for (auto &elem : children) {
    if (boost::apply_visitor(matcher, *elem)) matched.push_back(elem);
  }

  return {matched, *m_lib};
}

node node::up() {
  // Get parent for each element
  std::vector<ast::element_t *> parents;
  for (auto elem_ptr : m_elements)
    parents.push_back(m_lib->m_parents.at(elem_ptr));

  return {parents, *m_lib};
}

std::ostream &operator<<(std::ostream &out, const node &n) {
  out << "Node[";
  for (std::size_t i = 0; i < n.m_elements.size(); ++i) {
    if (i != 0) out << ", ";
    out << *n.m_elements[i];
  }
  out << "]";
  return out;
}

liberty::liberty(const boost::filesystem::path &file) : m_file(file) {
  if (!boost::filesystem::is_regular_file(file)) {
    throw std::invalid_argument{"File " +
                                boost::filesystem::absolute(file).string() +
                                " does not exists!"};
  }

  // Read file
  std::ifstream in{file.string()};
  const std::string source(std::istreambuf_iterator<char>(in), {});

  // Parse liberty
  liberty_grammar<std::string::const_iterator> grammar;
  skipper_grammar<std::string::const_iterator> skipper;

  ast::container_t root{"root", {}, {}};  // store libraries as elements
  auto it = source.begin(), end = source.end();
  const bool success =
      qi::phrase_parse(it, end, grammar, skipper, root.elements);

  if (!success || it != end) throw std::logic_error{"Parse unsuccessful!"};
  m_root = std::move(root);

  // Generate back references for walking up the tree
  generate_parents(m_parents, m_root);
  // For completeness, add root as its own parent
  m_parents.emplace(&m_root, &m_root);
}

node liberty::operator[](const std::regex &pattern) {
  node root{{&m_root}, *this};
  return root[pattern];
}

void liberty::write(const boost::filesystem::path &file) const {
  std::ofstream out{file.string()};
  printer print{out};
  auto &root = boost::get<ast::container_t>(m_root);
  for (auto &lib : root.elements) boost::apply_visitor(print, lib);
}

}  // namespace liberty