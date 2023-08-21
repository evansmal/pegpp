#pragma once

#include "parser.hpp"

auto Literal(const std::string &value) -> Parser;

auto Range(const std::string &start, const std::string &end) -> Parser;

auto Optional(const Parser &parser) -> Parser;

auto OneOrMore(const Parser &parser) -> Parser;

auto ZeroOrMore(const Parser &parser) -> Parser;

auto And(const Parser &parser) -> Parser;

template <typename... T>
auto Sequence(const T &...parsers) -> Parser
{
    return [](const auto &) { return Result{Success{{}, " "}}; };
}

template <typename... T>
auto Alternative(const T &...parsers) -> Parser
{
    return [](const auto &) { return Result{Success{{}, " "}}; };
}
