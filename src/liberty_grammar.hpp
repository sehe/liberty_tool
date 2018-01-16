#pragma once

#include "ast_types_adapted.hpp"
#include "error_handler.hpp"
#include "skipper_grammar.hpp"

#define BOOST_RESULT_OF_USE_DECLTYPE
#define BOOST_SPIRIT_USE_PHOENIX_V3

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace liberty {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename iterator>
struct liberty_grammar : qi::grammar<iterator, std::vector<ast::container_t>(),
                                     skipper_grammar<iterator>> {
  liberty_grammar() : liberty_grammar::base_type(libs) {
    libs = +container > qi::eoi;
    element = container | list | pair;
    container = name >> '(' >> -(arg % ',') >> ')' >> '{' >> +element > '}';
    list = name >> '(' >> (value % ',') >> ')' >> ';';
    pair = name >> ':' > value > ';';

    name = qi::lexeme[+(qi::alnum | qi::string("_"))];
    arg = qi::lexeme[+(qi::alnum | qi::string("_"))];
    value = unit | number | word | quoted;

    unit = qi::lexeme[qi::double_ >> +qi::alnum];
    number = qi::lexeme[qi::double_];
    // Workaround: No adapt for single member struct
    // https://stackoverflow.com/a/19824426/9224499
    word = qi::as_string[qi::lexeme[+(qi::alnum | qi::string("_"))]];
    quoted = qi::as_string[qi::lexeme['"' >> *(qi::char_ - '"') > '"']];

    element.name("element");      // debug(element);
    container.name("container");  // debug(container);
    list.name("list");            // debug(list);
    pair.name("pair");            // debug(pair);

    name.name("name");    // debug(name);
    arg.name("arg");      // debug(arg);
    value.name("value");  // debug(value);

    unit.name("unit");      // debug(unit);
    number.name("number");  // debug(number);
    word.name("word");      // debug(word);
    quoted.name("quoted");  // debug(quoted);

    // Print error message on parse failure.
    qi::on_error<qi::fail>(
        libs, boost::phoenix::bind(on_error, qi::_1, qi::_2, qi::_3, qi::_4));
  }

  qi::rule<iterator, std::vector<ast::container_t>(), skipper_grammar<iterator>>
      libs;
  qi::rule<iterator, ast::element_t(), skipper_grammar<iterator>> element;
  qi::rule<iterator, ast::container_t(), skipper_grammar<iterator>> container;
  qi::rule<iterator, ast::list_t(), skipper_grammar<iterator>> list;
  qi::rule<iterator, ast::pair_t(), skipper_grammar<iterator>> pair;

  qi::rule<iterator, std::string(), skipper_grammar<iterator>> name;
  qi::rule<iterator, std::string(), skipper_grammar<iterator>> arg;
  qi::rule<iterator, ast::value_t(), skipper_grammar<iterator>> value;

  qi::rule<iterator, ast::unit_t(), skipper_grammar<iterator>> unit;
  qi::rule<iterator, double(), skipper_grammar<iterator>> number;
  qi::rule<iterator, ast::word_t(), skipper_grammar<iterator>> word;
  qi::rule<iterator, ast::quoted_t(), skipper_grammar<iterator>> quoted;

  error_handler<iterator> on_error;
};

}  // namespace liberty