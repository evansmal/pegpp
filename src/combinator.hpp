#pragma once

#include "parser.hpp"

namespace combinator
{

auto Literal(const std::string &value) -> Parser;

auto Range(const std::string &start, const std::string &end) -> Parser;

auto Sequence(const std::vector<Parser> &parsers) -> Parser;

auto Alternative(const std::vector<Parser> &parsers) -> Parser;

auto Optional(const Parser &parser) -> Parser;

auto OneOrMore(const Parser &parser) -> Parser;

auto ZeroOrMore(const Parser &parser) -> Parser;

auto And(const Parser &parser) -> Parser;

auto Not(const Parser &parser) -> Parser;

auto Dot() -> Parser;

auto Definition(const Parser &parser, const std::string &type) -> Parser;

}; // namespace combinator
