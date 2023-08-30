#include <catch2/catch_test_macros.hpp>

#include "helpers.hpp"

#include "ast.hpp"
#include "generator.hpp"

TEST_CASE("Generate and use collection", "[Generate]")
{
    SECTION("Generate empty collection")
    {
        const auto ast = ast::Grammar();
        const auto collection = Generate(ast);
    }
    SECTION("Generate simple collection")
    {
        const auto ast =
            ast::Grammar(ast::Definition(ast::Identifier("Zero"), ast::Literal("0")),
                         ast::Definition(ast::Identifier("One"), ast::Literal("1")),
                         ast::Definition(ast::Identifier("ZeroThenOne"),
                                         ast::Sequence(ast::Identifier("Zero"),
                                                       ast::Identifier("One"))));
        const auto collection = Generate(ast);

        {
            const auto result = collection.Get("Zero")("0");
            REQUIRE(UnwrapSuccess(result).remainder.empty());
            REQUIRE(UnwrapSuccess(result).node.size() == 1);
        }
        {
            const auto result = collection.Get("One")("1");
            REQUIRE(UnwrapSuccess(result).remainder.empty());
            REQUIRE(UnwrapSuccess(result).node.size() == 1);
        }
        {
            const auto result = collection.Get("ZeroThenOne")("01");
            REQUIRE(UnwrapSuccess(result).remainder.empty());
            REQUIRE(UnwrapSuccess(result).node.size() == 1);

            const auto root = UnwrapNonTerminal(UnwrapSuccess(result).node[0]);
            REQUIRE(root.type == "ZeroThenOne");
            REQUIRE(root.children.size() == 2);
            REQUIRE(UnwrapNonTerminal(root.children[0]).type == "Zero");
            REQUIRE(UnwrapNonTerminal(root.children[1]).type == "One");
        }
    }
}
