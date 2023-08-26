#include "generator.hpp"
#include "combinator.hpp"

template <typename> inline constexpr bool AlwaysFalse = false;

auto EmitExpression(const ast::Expression &expression, ParserMap &parser_map) -> Parser
{
    return std::visit(
        [&parser_map](auto &&expression)
        {
            using T = std::decay_t<decltype(expression)>;
            if constexpr (std::is_same_v<T, ast::Sequence>)
            {
                std::vector<Parser> parsers;
                parsers.reserve(expression.children.size());
                for (const ast::Expression &child : expression.children)
                {
                    parsers.push_back(EmitExpression(child, parser_map));
                }
                return Sequence(parsers);
            }
            else if constexpr (std::is_same_v<T, ast::Literal>)
            {
                return Literal(expression.value);
            }
            else if constexpr (std::is_same_v<T, ast::Identifier>)
            {
                Parser parser = [&](const std::string &input)
                {
                    auto res = parser_map.at(expression.value)(input);
                    return res;
                };
                return parser;
            }
            else if constexpr (std::is_same_v<T, ast::Range>)
            {
                return Range(expression.start, expression.end);
            }
            else
            {
                static_assert(AlwaysFalse<T>, "Did not visit all possible cases");
            }
        },
        expression);
}

auto Generate(const ast::Grammar &grammar) -> ParserMap
{
    std::map<std::string, Parser> parsers;
    for (const auto &definition : grammar.definitions)
    {
        const auto expr = EmitExpression(definition.expression, parsers);
        const auto def = Definition(expr, definition.identifier.value);
        parsers[definition.identifier.value] = def;
    }
    return parsers;
}
