#include "ast.hpp"
#include "combinator.hpp"
#include "parser.hpp"

#include <iostream>
#include <map>

template <typename> inline constexpr bool AlwaysFalse = false;

using ParserMap = std::map<std::string, Parser>;

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
                    std::cout << "Parsing " << expression.value << std::endl;
                    std::cout << input << std::endl;
                    for (const auto [f, s] : parser_map)
                    {
                        std::cout << f << std::endl;
                    }
                    std::cout << " TRYING IT" << std::endl;
                    auto res = parser_map.at(expression.value)(input);
                    std::cout << " GOT IT" << std::endl;
                    return res;
                };
                return parser;
            }
            else
            {
                static_assert(AlwaysFalse<T>, "Did not visit all possible cases");
            }
        },
        expression);
}

auto GenerateParserMap(const ast::Grammar &grammar) -> ParserMap
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

int main()
{
    auto g = ast::Grammar(
        ast::Definition(ast::Identifier("Test"),
                        ast::Sequence(ast::Identifier("Plus"), ast::Identifier("Minus"))),
        ast::Definition(ast::Identifier("Plus"), ast::Literal("+")),
        ast::Definition(ast::Identifier("Minus"), ast::Literal("-")));

    g.Dump();

    const auto parser = GenerateParserMap(g);

    const auto result = parser.at("Test")("+-");
    if (std::holds_alternative<Success>(result))
    {
        std::cout << "Parser success!" << std::endl;
        const auto &success = std::get<Success>(result);
        for (const auto &node : success.node)
        {
            Dump(node);
        }
    }
    else
    {
        std::cout << "Parse failed!" << std::endl;
    }

    return 0;
}
