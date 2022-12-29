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

TEST_CASE("AndThen", "[AndThen]")
{
    SECTION("Handle single values")
    {
        const auto parser = AndThen(Literal('a'), Literal('b'));
        const std::tuple<char, char> result = UnwrapValue(parser("ab"));
        REQUIRE(std::get<0>(result) == 'a');
        REQUIRE(std::get<1>(result) == 'b');
    }
    SECTION("Fail to parse")
    {
        const auto parser = AndThen(Literal('a'), Literal('b'));
        REQUIRE(UnwrapResult(parser("qwert")) == false);
    }
}

TEST_CASE("Parse single digits", "[Digit]")
{
    SECTION("Handle single values")
    {
        REQUIRE(UnwrapValue(Digit()("0")) == '0');
        REQUIRE(UnwrapValue(Digit()("123")) == '1');
        REQUIRE(UnwrapValue(Digit()("321")) == '3');
    }
    SECTION("Fail to parse")
    {
        REQUIRE(UnwrapResult(Digit()("A")) == false);
        REQUIRE(UnwrapResult(Digit()("B")) == false);
        REQUIRE(UnwrapResult(Digit()("{}{}{}")) == false);
    }
}
