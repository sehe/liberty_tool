#pragma once

#define BOOST_RESULT_OF_USE_DECLTYPE
#define BOOST_SPIRIT_USE_PHOENIX_V3

#include <boost/spirit/include/qi.hpp>

namespace liberty {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename iterator>
struct skipper_grammar : qi::grammar<iterator> {
  skipper_grammar() : skipper_grammar::base_type(skip) {
    skip = ascii::space | comment | escaped;
    comment = qi::lit("/*") >> *(qi::char_ - "*/") >> "*/";
    escaped = '\\' >> qi::eol;
  }

  qi::rule<iterator> skip;
  qi::rule<iterator> comment;
  qi::rule<iterator> escaped;
};

}  // namespace liberty