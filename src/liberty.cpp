#include "liberty.hpp"

#include "liberty_grammar.hpp"
#include "skipper_grammar.hpp"

#include <fstream>
#include <string>

namespace liberty {

liberty::liberty(const boost::filesystem::path &file) : m_file(file) {
    // read file
    std::ifstream in(file.string());
    const std::string lib(std::istreambuf_iterator<char>(in), {});

    liberty_grammar<std::string::const_iterator> grammar;
    skipper_grammar<std::string::const_iterator> skipper;

    // parse liberty
    auto it = lib.begin(), end = lib.end();
    const bool success = qi::phrase_parse(it, end, grammar, skipper, m_root);

    if (!success || it != end)
        throw std::logic_error("Parse unsuccessful!");
}

} // namespace liberty