#include <catch2/catch_test_macros.hpp>

#include "../src/ccombinators.h"

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
        REQUIRE(UnwrapValue(Literal('a')("aaa")) == 'a');
        REQUIRE(UnwrapValue(Literal('b')("baa")) == 'b');
    }
}
