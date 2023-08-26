#pragma once

#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace ast
{

struct Identifier;
struct Literal;
struct Range;
struct Sequence;

using Expression = std::variant<Sequence, Range, Literal, Identifier>;

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
