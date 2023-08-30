#pragma once

#include "box.hpp"

#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace ast
{

using Expression = std::variant<Box<struct Sequence>,
                                Box<struct Alternative>,
                                Box<struct Optional>,
                                Box<struct OneOrMore>,
                                Box<struct ZeroOrMore>,
                                Box<struct And>,
                                Box<struct Not>,
                                struct Dot,
                                struct Range,
                                struct Literal,
                                struct Identifier>;

struct Identifier
{
    Identifier(std::string value) : value{std::move(value)} {}
    std::string value;
};

struct Literal
{
    Literal(std::string value) : value{std::move(value)} {}
    std::string value;
};

struct Range
{
    Range(std::string start, std::string end)
        : start{std::move(start)}, end{std::move(end)}
    {
    }
    std::string start;
    std::string end;
};

struct Dot
{
    Dot() = default;
};

struct Optional
{
    Optional(Expression child) : child{std::move(child)} {}
    Expression child;
};

struct ZeroOrMore
{
    ZeroOrMore(Expression child) : child{std::move(child)} {}
    Expression child;
};

struct OneOrMore
{
    OneOrMore(Expression child) : child{std::move(child)} {}
    Expression child;
};

struct And
{
    And(Expression child) : child{std::move(child)} {}
    Expression child;
};

struct Not
{
    Not(Expression child) : child{std::move(child)} {}
    Expression child;
};

struct Alternative
{
    template <typename... Children>
    Alternative(Children &&...children) : children{children...}
    {
    }
    std::vector<Expression> children;
};

struct Sequence
{
    template <typename... Children>
    Sequence(Children &&...children) : children{children...}
    {
    }
    std::vector<Expression> children;
};

struct Definition
{
    Definition(Identifier identifier, Expression expression)
        : identifier{std::move(identifier)}, expression{std::move(expression)}
    {
    }
    Identifier identifier;
    Expression expression;
};

struct Grammar
{
    template <typename... Definitions>
    Grammar(Definitions &&...definitions) : definitions{definitions...}
    {
    }
    std::vector<Definition> definitions;
};

} // namespace ast
