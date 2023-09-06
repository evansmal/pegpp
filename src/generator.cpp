#include "generator.hpp"
#include "combinator.hpp"

#include "box.hpp"

template <typename> inline constexpr bool AlwaysFalse = false;

auto EmitExpression(const ast::Expression &expression, Collection &collection) -> Parser
{
    return std::visit(
        [&collection](auto &&expression)
        {
            using T = std::decay_t<decltype(expression)>;
            if constexpr (std::is_same_v<T, Box<ast::Sequence>>)
            {
                std::vector<Parser> parsers;
                parsers.reserve(expression->children.size());
                for (const ast::Expression &child : expression->children)
                {
                    parsers.push_back(EmitExpression(child, collection));
                }
                return combinator::Sequence(parsers);
            }
            else if constexpr (std::is_same_v<T, Box<ast::Optional>>)
            {
                return combinator::Optional(
                    EmitExpression(expression->child, collection));
            }
            else if constexpr (std::is_same_v<T, Box<ast::ZeroOrMore>>)
            {
                return combinator::ZeroOrMore(
                    EmitExpression(expression->child, collection));
            }
            else if constexpr (std::is_same_v<T, Box<ast::OneOrMore>>)
            {
                return combinator::OneOrMore(
                    EmitExpression(expression->child, collection));
            }
            else if constexpr (std::is_same_v<T, Box<ast::And>>)
            {
                return combinator::And(EmitExpression(expression->child, collection));
            }
            else if constexpr (std::is_same_v<T, Box<ast::Not>>)
            {
                return combinator::Not(EmitExpression(expression->child, collection));
            }
            else if constexpr (std::is_same_v<T, Box<ast::Alternative>>)
            {
                std::vector<Parser> parsers;
                parsers.reserve(expression->children.size());
                for (const ast::Expression &child : expression->children)
                {
                    parsers.push_back(EmitExpression(child, collection));
                }
                return combinator::Alternative(parsers);
            }
            else if constexpr (std::is_same_v<T, ast::Class>)
            {
                const ast::Class &cls = expression;

                std::vector<Parser> parsers;
                parsers.reserve(cls.literals.size() + cls.ranges.size());
                for (const auto &literal : cls.literals)
                {
                    parsers.push_back(EmitExpression(literal, collection));
                }
                for (const auto &range : cls.ranges)
                {
                    parsers.push_back(EmitExpression(range, collection));
                }
                return combinator::Alternative(parsers);
            }
            else if constexpr (std::is_same_v<T, ast::Dot>)
            {
                return combinator::Dot();
            }
            else if constexpr (std::is_same_v<T, ast::Literal>)
            {
                return combinator::Literal(expression.value);
            }
            else if constexpr (std::is_same_v<T, ast::Identifier>)
            {
                Parser parser = [&](const std::string &input)
                {
                    auto res = collection.Get(expression.value)(input);
                    return res;
                };
                return parser;
            }
            else if constexpr (std::is_same_v<T, ast::Range>)
            {
                return combinator::Range(expression.start, expression.end);
            }
            else
            {
                static_assert(AlwaysFalse<T>, "Did not visit all possible cases");
            }
        },
        expression);
}

auto Generate(const ast::Grammar &grammar) -> Collection
{
    Collection collection;
    for (const auto &definition : grammar.definitions)
    {
        const auto expr = EmitExpression(definition.expression, collection);
        const auto def = combinator::Definition(expr, definition.identifier.value);
        collection.Add(definition.identifier.value, def);
    }
    return collection;
}
