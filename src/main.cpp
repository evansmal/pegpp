#include "ast.hpp"
#include "combinator.hpp"
#include "generator.hpp"
#include "grammar.hpp"
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

    const auto peg = Generate(GRAMMAR);

    const std::string grammar = "Expression <- Number BinaryOperand Number\n"
                                "BinaryOperand <- Plus / Minus\n"
                                "Plus <- '+'\n"
                                "Minus <- '-'\n"
                                "Number <- [0-9]\n";

    const auto parse = peg.Get("Grammar")(grammar);
    if (std::holds_alternative<Success>(parse))
    {
        std::cout << "Parser success!" << std::endl;
        const auto &success = std::get<Success>(parse);
        for (const auto &node : success.node)
        {
            Dump(node);
        }
    }
    else
    {
        std::cout << "Parser failure!" << std::endl;
    }

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
