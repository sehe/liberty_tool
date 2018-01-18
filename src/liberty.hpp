#pragma once

#include <boost/filesystem.hpp>
#include <ostream>
#include <regex>
#include "ast_types.hpp"

namespace liberty {

class node {
 public:
  node(std::vector<ast::element_t *> elements);

  node operator[](const std::regex &pattern);
  node operator[](const std::string &pattern) {
    return operator[](std::regex{pattern});
  }

  auto begin() { return m_elements.begin(); }
  auto end() { return m_elements.end(); }
  std::size_t size() const { return m_elements.size(); }

  friend std::ostream &operator<<(std::ostream &, const node &);

 private:
  std::vector<ast::element_t *> m_elements;
};

// Reads and writes liberty files.
class liberty {
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
};

}  // namespace liberty