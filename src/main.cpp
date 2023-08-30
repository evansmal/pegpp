#include "ast.hpp"
#include "combinator.hpp"
#include "generator.hpp"
#include "parser.hpp"

#include <iostream>
#include <map>

auto main() -> int
{
    auto g = ast::Grammar(
        ast::Definition(ast::Identifier("Expression"),
                        Box(ast::Sequence(ast::Identifier("Number"),
                                          ast::Identifier("BinaryOperand"),
                                          ast::Identifier("Number")))),
        ast::Definition(
            ast::Identifier("BinaryOperand"),
            Box(ast::Alternative(ast::Identifier("Plus"), ast::Identifier("Minus")))),
        ast::Definition(ast::Identifier("Plus"), ast::Literal("+")),
        ast::Definition(ast::Identifier("Minus"), ast::Literal("-")),
        ast::Definition(ast::Identifier("Number"), ast::Range("0", "9")));

    std::cout << "Created grammar" << std::endl;

    const auto collection = Generate(g);

    std::cout << "Created collection" << std::endl;

    const auto result = collection.Get("Expression")("1+2");
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
        std::cout << "Parser failure!" << std::endl;
    }

    return 0;
}
