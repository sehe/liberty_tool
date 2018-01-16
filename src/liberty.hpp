#pragma once

#include <boost/filesystem.hpp>
#include <regex>
#include "ast_types.hpp"

namespace liberty {

class node {
 public:
  node(std::vector<ast::element_t *> elems);

  node operator[](const std::regex &regex);
  node operator[](const std::string &regex) {
    return operator[](std::regex{regex});
  }

  std::size_t size() const { return m_elems.size(); }

 private:
  std::vector<ast::element_t *> m_elems;
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