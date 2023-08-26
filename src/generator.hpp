#pragma once

#include "ast.hpp"
#include "parser.hpp"

using ParserMap = std::map<std::string, Parser>;

auto Generate(const ast::Grammar &grammar) -> ParserMap;
