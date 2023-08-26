#include "ast.hpp"
#include "combinator.hpp"
#include "generator.hpp"
#include "parser.hpp"

#include <iostream>
#include <map>

auto main() -> int
{
    auto g = ast::Grammar(
        ast::Definition(ast::Identifier("Test"),
                        ast::Sequence(ast::Identifier("Plus"), ast::Identifier("Minus"))),
        ast::Definition(ast::Identifier("Plus"), ast::Literal("+")),
        ast::Definition(ast::Identifier("Minus"), ast::Literal("-")));

    const auto parser = Generate(g);

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
    }

    return 0;
}
