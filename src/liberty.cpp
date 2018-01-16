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
class matcher : public boost::static_visitor<bool> {
 public:
  matcher(const std::regex &rgx) : regex(rgx) {}

  bool operator()(const ast::container_t &container) const {
    std::vector<std::string> ids;
    for (auto &arg : container.args) ids.push_back(container.name + ':' + arg);
    return std::any_of(ids.begin(), ids.end(), [&](const std::string &id) {
      return std::regex_match(id, regex);
    });
  }

  bool operator()(const ast::list_t &list) const {
    return std::regex_match(list.name, regex);
  }

  bool operator()(const ast::pair_t &pair) const {
    return std::regex_match(pair.name, regex);
  }

 private:
  const std::regex regex;
};
}

node::node(std::vector<ast::element_t *> elems) : m_elems(elems) {}

node node::operator[](const std::regex &regex) {
  // Filter elements by regex
  matcher match{regex};
  std::vector<ast::element_t *> matched;

  for (auto &elem : m_elems) {
    if (boost::apply_visitor(match, *elem)) matched.push_back(elem);
  }

  return matched;
}

liberty::liberty(const boost::filesystem::path &file) : m_file(file) {
  if (!boost::filesystem::is_regular_file(file))
    throw std::invalid_argument{"File " +
                                boost::filesystem::absolute(file).string() +
                                " does not exists!"};

  // Read file
  std::ifstream in{file.string()};
  const std::string source(std::istreambuf_iterator<char>(in), {});

  // Parse liberty
  liberty_grammar<std::string::const_iterator> grammar;
  skipper_grammar<std::string::const_iterator> skipper;

  ast::container_t root{"root"};  // store libraries as elements
  auto it = source.begin(), end = source.end();
  const bool success =
      qi::phrase_parse(it, end, grammar, skipper, root.elements);

  if (!success || it != end) throw std::logic_error{"Parse unsuccessful!"};
  m_root = std::move(root);
}

node liberty::operator[](const std::regex &regex) {
  node root{{&m_root}};
  return root[regex];
}

void liberty::write(const boost::filesystem::path &file) const {
  std::ofstream out{file.string()};
  printer print{out};
  auto &root = boost::get<ast::container_t>(m_root);
  for (auto &lib : root.elements) boost::apply_visitor(print, lib);
}

}  // namespace liberty