#include <catch2/catch_test_macros.hpp>

#include "combinator.hpp"

auto UnwrapSuccess(const Result &result) -> Success
{
    if (std::holds_alternative<Success>(result))
    {
        return std::get<Success>(result);
    }
    throw std::runtime_error("Failed to unwrap parser success");
}

auto UnwrapFailure(const Result &result) -> Failure
{
    if (std::holds_alternative<Failure>(result))
    {
        return std::get<Failure>(result);
    }
    const auto success = std::get<Success>(result);
    throw std::runtime_error("Failed to unwrap parser failure. Remainder: " + success.remainder);
}

auto UnwrapTerminal(const Node &node) -> Terminal
{
    if (std::holds_alternative<Terminal>(node))
    {
        return std::get<Terminal>(node);
    }
    throw std::runtime_error("Failed to unwrap Terminal");
}

auto UnwrapNonTerminal(const Node &node) -> NonTerminal
{
    if (std::holds_alternative<NonTerminal>(node))
    {
        return std::get<NonTerminal>(node);
    }
    throw std::runtime_error("Failed to unwrap NonTerminal");
}

TEST_CASE("Literals are parsed", "[Literal]")
{
    SECTION("Handle single values")
    {
        {
            auto res = UnwrapSuccess(Literal("a")("a"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "a");
        }
        {
            auto res = UnwrapSuccess(Literal("0")("0123"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.size() == 3);
            REQUIRE(res.remainder == "123");
            REQUIRE(UnwrapTerminal(res.node[0]).value == "0");
        }
    }
    SECTION("Handle multiple values")
    {
        {
            auto res = UnwrapSuccess(Literal("ABC")("ABC"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "ABC");
        }
        {
            auto res = UnwrapSuccess(Literal("123")("123456"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.size() == 3);
            REQUIRE(res.remainder == "456");
            REQUIRE(UnwrapTerminal(res.node[0]).value == "123");
        }
    }
    SECTION("Expect failure")
    {
        REQUIRE_NOTHROW(UnwrapFailure(Literal("X")("Y")));
        REQUIRE_NOTHROW(UnwrapFailure(Literal("XY")("YX")));
        REQUIRE_NOTHROW(UnwrapFailure(Literal("A")("")));
    }
}

TEST_CASE("Ranges are parsed", "[Range]")
{
    SECTION("Handle range values")
    {
        {
            auto res = UnwrapSuccess(Range("0", "9")("0"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "0");
        }
        {
            auto res = UnwrapSuccess(Range("0", "9")("9"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "9");
        }
        {
            auto res = UnwrapSuccess(Range("a", "z")("f"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "f");
        }
        {
            auto res = UnwrapSuccess(Range("0", "9")("5678"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.remainder.size() == 3);
            REQUIRE(res.remainder == "678");
            REQUIRE(UnwrapTerminal(res.node[0]).value == "5");
        }
    }
    SECTION("Expect failure") { REQUIRE_NOTHROW(UnwrapFailure(Range("0", "1")("2"))); }
}

TEST_CASE("Sequence parsers", "[Sequence]")
{
    SECTION("Parse sequence of literals")
    {
        {
            auto res = UnwrapSuccess(Sequence({Literal("A"), Literal("B"), Literal("C")})("ABC"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 3);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "A");
            REQUIRE(UnwrapTerminal(res.node[1]).value == "B");
            REQUIRE(UnwrapTerminal(res.node[2]).value == "C");
        }
    }
    SECTION("Expect failure")
    {
        REQUIRE_NOTHROW(UnwrapFailure(Sequence({Literal("0"), Literal("1"), Literal("2")})("092")));
    }
}

TEST_CASE("Alternative parsers", "[Alternative]")
{
    SECTION("Parse alternative set of literals")
    {
        {
            auto res = UnwrapSuccess(Alternative({Literal("A"), Literal("B"), Literal("C")})("C"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 1);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "C");
        }
        {
            auto res = UnwrapSuccess(Alternative({Literal("2"), Literal("1")})("123"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 1);
            REQUIRE(res.remainder.size() == 2);
            REQUIRE(UnwrapTerminal(res.node[0]).value == "1");
        }
    }
    SECTION("Expect failure")
    {
        REQUIRE_NOTHROW(UnwrapFailure(Alternative({Literal("0")})("1")));
        REQUIRE_NOTHROW(
            UnwrapFailure(Alternative({Literal("0"), Literal("1"), Literal("2")})("3")));
    }
}

TEST_CASE("Optional parsers", "[Optional]")
{
    SECTION("Parse optionals")
    {
        {
            auto res = UnwrapSuccess(Optional(Literal("1"))("1"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 1);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "1");
        }
        {
            auto res = UnwrapSuccess(Optional(Literal("A"))("C"));
            REQUIRE(res.node.empty());
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 1);
        }
    }
}

TEST_CASE("One or more parsers", "[OneOrMore]")
{
    SECTION("Parse one or more literals")
    {
        {
            auto res = UnwrapSuccess(OneOrMore(Literal("1"))("1"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 1);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "1");
        }
        {
            auto res = UnwrapSuccess(OneOrMore(Literal("1"))("111"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 3);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "1");
            REQUIRE(UnwrapTerminal(res.node[1]).value == "1");
            REQUIRE(UnwrapTerminal(res.node[2]).value == "1");
        }
    }
    SECTION("Fail to parse a single literal")
    {
        REQUIRE_NOTHROW(UnwrapFailure(OneOrMore(Literal("1"))("0")));
        REQUIRE_NOTHROW(UnwrapFailure(OneOrMore(Literal("1"))("321")));
    }
}

TEST_CASE("Zero or more parsers", "[ZeroOrMore]")
{
    SECTION("Parse zero or more literals")
    {
        {
            auto res = UnwrapSuccess(ZeroOrMore(Literal("1"))("1"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 1);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "1");
        }
        {
            auto res = UnwrapSuccess(ZeroOrMore(Literal("1"))("111"));
            REQUIRE(!res.node.empty());
            REQUIRE(res.node.size() == 3);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapTerminal(res.node[0]).value == "1");
            REQUIRE(UnwrapTerminal(res.node[1]).value == "1");
            REQUIRE(UnwrapTerminal(res.node[2]).value == "1");
        }
        {
            auto res = UnwrapSuccess(ZeroOrMore(Literal("1"))("0"));
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 1);
        }
        {
            auto res = UnwrapSuccess(ZeroOrMore(Literal("A"))("98765"));
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 5);
        }
        {
            auto res = UnwrapSuccess(ZeroOrMore(Literal("Z"))("ABC"));
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 3);
        }
    }
}

TEST_CASE("And parser", "[And]")
{
    SECTION("Parse AND literals")
    {
        {
            auto res = UnwrapSuccess(And(Literal("1"))("1"));
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 1);
        }
    }
}

TEST_CASE("Not parser", "[Not]")
{
    SECTION("Parse NOT literals")
    {
        {
            auto res = UnwrapSuccess(Not(Literal("1"))("0"));
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 1);
        }
        {
            auto res = UnwrapSuccess(Not(Literal("1"))("ABC"));
            REQUIRE(res.node.empty());
            REQUIRE(res.remainder.size() == 3);
        }
    }
}

TEST_CASE("Definition parser", "[Definition]")
{
    SECTION("Parse definitions")
    {
        {
            auto res = UnwrapSuccess(Definition(Literal("0"), "ZERO")("0"));
            REQUIRE(res.node.size() == 1);
            REQUIRE(res.remainder.empty());
            REQUIRE(UnwrapNonTerminal(res.node[0]).children.size() == 1);
            REQUIRE(UnwrapNonTerminal(res.node[0]).type == "ZERO");
        }
    }
}
