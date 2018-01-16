#include "liberty.hpp"

#include <fstream>
#include <string>
#include "liberty_grammar.hpp"
#include "printer.hpp"
#include "skipper_grammar.hpp"

namespace liberty {

liberty::liberty(const boost::filesystem::path &file) : m_file(file) {
  // Read file
  std::ifstream in{file.string()};
  const std::string lib(std::istreambuf_iterator<char>(in), {});

  // Parse liberty
  liberty_grammar<std::string::const_iterator> grammar;
  skipper_grammar<std::string::const_iterator> skipper;

  auto it = lib.begin(), end = lib.end();
  const bool success = qi::phrase_parse(it, end, grammar, skipper, m_root);

  if (!success || it != end) throw std::logic_error{"Parse unsuccessful!"};
}

void liberty::write(const boost::filesystem::path &file) const {
  std::ofstream out{file.string()};
  printer print{out};
  print(m_root);
}

}  // namespace liberty