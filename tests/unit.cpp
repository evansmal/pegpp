#include <catch2/catch_test_macros.hpp>

#include "../src/ccombinators.h"

template <typename T>
std::string UnwrapRest(const ParseResult<T> &result)
{
    if (std::holds_alternative<ParseSuccess<T>>(result))
    {
        return std::get<ParseSuccess<T>>(result).rest;
    }
    else
    {
        throw std::runtime_error("ParserFailure");
    }
}

template <typename T>
T UnwrapValue(const ParseResult<T> &result)
{
    if (std::holds_alternative<ParseSuccess<T>>(result))
    {
        return std::get<ParseSuccess<T>>(result).value;
    }
    else
    {
        throw std::runtime_error("ParserFailure");
    }
}

template <typename T>
bool UnwrapResult(const ParseResult<T> &result)
{
    return std::holds_alternative<ParseSuccess<T>>(result);
}

TEST_CASE("Literals are parsed", "[Literal]")
{
    SECTION("Handle single values")
    {
        REQUIRE(UnwrapValue(Literal('a')("a")) == 'a');
        REQUIRE(UnwrapValue(Literal('0')("0")) == '0');
        REQUIRE(UnwrapValue(Literal('{')("{")) == '{');
    }
    SECTION("Fail to parse")
    {
        REQUIRE(UnwrapResult(Literal('a')("b")) == false);
        REQUIRE(UnwrapResult(Literal('b')("a")) == false);
        REQUIRE(UnwrapResult(Literal('0')("1")) == false);
    }
    SECTION("Handle literals values in larger string")
    {
        {
            const auto parser = Literal('a');
            const auto result = parser("aaa");
            REQUIRE(UnwrapValue(result) == 'a');
            REQUIRE(UnwrapRest(result) == "aa");
        }
        {
            const auto parser = Literal('b');
            const auto result = parser("bcde");
            REQUIRE(UnwrapValue(result) == 'b');
            REQUIRE(UnwrapRest(result) == "cde");
        }
    }
}

TEST_CASE("OrElse", "[OrElse]")
{
    SECTION("Handle single values")
    {
        const auto parser = OrElse(Literal('a'), Literal('b'));
        REQUIRE(UnwrapValue(parser("a")) == 'a');
        REQUIRE(UnwrapValue(parser("b")) == 'b');
    }
    SECTION("Fail to parse")
    {
        const auto parser = OrElse(Literal('0'), Literal('1'));
        REQUIRE(UnwrapResult(parser("321")) == false);
    }
}
