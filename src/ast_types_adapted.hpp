#pragma once

#include <boost/fusion/include/adapt_struct.hpp>
#include "ast_types.hpp"

// clang-format off

// Simple types
BOOST_FUSION_ADAPT_STRUCT(
    liberty::ast::unit_t,
    (double, number)
    (std::string, unit))

// Workaround: No adapt for single member structs
// https://stackoverflow.com/a/19824426/9224499
//BOOST_FUSION_ADAPT_STRUCT(
//    liberty::ast::word_t,
//    (std::string, string))

//BOOST_FUSION_ADAPT_STRUCT(
//    liberty::ast::quoted_t,
//    (std::string, string))

// Complex types
BOOST_FUSION_ADAPT_STRUCT(
    liberty::ast::container_t,
    (std::string, name)
    (std::vector<std::string>, args)
    (std::vector<liberty::ast::element_t>, elements))

BOOST_FUSION_ADAPT_STRUCT(
    liberty::ast::list_t,
    (std::string, name)
    (std::vector<liberty::ast::value_t>, values))

BOOST_FUSION_ADAPT_STRUCT(
    liberty::ast::pair_t,
    (std::string, name)
    (liberty::ast::value_t, value))

// clang-format on