#pragma once

#include <boost/filesystem.hpp>
#include <map>
#include <ostream>
#include <regex>
#include "ast_types.hpp"

namespace liberty {

class liberty;

class node {
 public:
  node(std::vector<ast::element_t *> elements, const liberty &lib);

  node operator[](const std::regex &pattern);
  node operator[](const std::string &pattern) {
    return operator[](std::regex{pattern});
  }

  node up();

  auto begin() { return m_elements.begin(); }
  auto end() { return m_elements.end(); }
  std::size_t size() const { return m_elements.size(); }

  friend std::ostream &operator<<(std::ostream &out, const node &n);

 private:
  std::vector<ast::element_t *> m_elements;
  const liberty *m_lib;
};

// Reads and writes liberty files.
class liberty {
  friend class node;

 public:
  liberty(const boost::filesystem::path &file);

  node operator[](const std::regex &pattern);
  node operator[](const std::string &pattern) {
    return operator[](std::regex{pattern});
  }

  boost::filesystem::path file() const { return m_file; };

  void write(const boost::filesystem::path &file) const;

 private:
  boost::filesystem::path m_file;
  ast::element_t m_root;  // holds all libraries
  std::map<ast::element_t *, ast::element_t *> m_parents;  // look up parents
};

}  // namespace liberty