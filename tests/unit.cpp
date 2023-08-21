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
    throw std::runtime_error("Failed to unwrap parser failure");
}

auto UnwrapTerminal(const Node &node) -> Terminal
{
    if (std::holds_alternative<Terminal>(node))
    {
        return std::get<Terminal>(node);
    }
    throw std::runtime_error("Failed to unwrap Terminal");
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
    SECTION("Fail to parse")
    {
        UnwrapFailure(Literal("X")("Y"));
        UnwrapFailure(Literal("XY")("YX"));
        UnwrapFailure(Literal("A")(""));
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
}
