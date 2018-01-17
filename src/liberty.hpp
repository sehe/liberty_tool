#pragma once

#include <boost/filesystem.hpp>
#include <ostream>
#include <regex>
#include "ast_types.hpp"

namespace liberty {

class node {
 public:
  node(std::vector<ast::element_t *> elements);

  node operator[](const std::regex &regex);
  node operator[](const std::string &regex) {
    return operator[](std::regex{regex});
  }

  auto begin() { return m_elements.begin(); }
  auto end() { return m_elements.end(); }
  std::size_t size() const { return m_elements.size(); }

  friend std::ostream &operator<<(std::ostream &, const node &);

 private:
  std::vector<ast::element_t *> m_elements;
};

class liberty {
 public:
  liberty(const boost::filesystem::path &file);

  node operator[](const std::regex &regex);
  node operator[](const std::string &regex) {
    return operator[](std::regex{regex});
  }

  boost::filesystem::path file() const { return m_file; };

  void write(const boost::filesystem::path &file) const;

 private:
  boost::filesystem::path m_file;
  ast::element_t m_root;  // holds all libraries
};

}  // namespace liberty