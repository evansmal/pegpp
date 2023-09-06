#include "ast.hpp"
#include "combinator.hpp"
#include "generator.hpp"
#include "grammar.hpp"
#include "parser.hpp"

#include <iostream>
#include <map>

auto CreatePegParser(const std::string &grammar) -> void
{
    const auto peg_grammar = GetPegGrammarAST();
    const auto peg = Generate(peg_grammar);
    const auto parse = peg.Parse("Grammar", grammar);
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
}

auto main() -> int
{
    using namespace ast;

    const std::string grammar = "Expression <- Number BinaryOperand Number\n"
                                "BinaryOperand <- Plus / Minus\n"
                                "Plus <- '+'\n"
                                "Minus <- '-'\n"
                                "Number <- [0-9]\n";
    CreatePegParser(grammar);

    auto grammar_ast =
        Grammar(Definition(Identifier("Expression"),
                           Box(Sequence(Identifier("Number"),
                                        Identifier("BinaryOperand"),
                                        Identifier("Number")))),
                Definition(Identifier("BinaryOperand"),
                           Box(Alternative(Identifier("Plus"), Identifier("Minus")))),
                Definition(Identifier("Plus"), Literal("+")),
                Definition(Identifier("Minus"), Literal("-")),
                Definition(Identifier("Number"), Range("0", "9")));

    std::cout << "Created grammar" << std::endl;
    const auto collection = Generate(grammar_ast);

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
