#pragma once

#include "ast_types.hpp"
#include <boost/filesystem.hpp>

namespace liberty {
    
class liberty {
public:
    liberty(const boost::filesystem::path &file);

    boost::filesystem::path file() const { return m_file; };
    const ast::container &root() const { return m_root; };

private:
    boost::filesystem::path m_file;
    ast::container          m_root;
};

} // namespace liberty