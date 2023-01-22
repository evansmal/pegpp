#pragma once

#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>

template <typename T>
struct ParseSuccess
{
    T value;
    std::string_view rest;
};

struct ParseFailure
{
    std::string_view message;
};

template <typename T>
using ParseResult = std::variant<ParseSuccess<T>, ParseFailure>;

template <typename T>
using Parser = std::function<ParseResult<T>(const std::string_view &)>;

template <typename T, typename U>
using Tuple = std::tuple<T, U>;

Parser<char> Literal(char c)
{
    return [c](const std::string_view &input) {
        if (input[0] == c)
        {
            return ParseResult<char>(
                ParseSuccess<char>{char(input[0]), {input.cbegin() + 1, input.cend()}});
        }
        else
        {
            std::stringstream ss;
            ss << "Expected '" << int(c) << "' but got '" << input[0] << "'";
            return ParseResult<char>(ParseFailure{ss.str()});
        }
    };
}

template <typename T, typename U>
Parser<Tuple<T, U>> AndThen(const Parser<T> &a, const Parser<U> &b)
{
    return [a, b](const std::string_view &input) {
        const auto a_result = a(input);
        if (std::holds_alternative<ParseSuccess<T>>(a_result))
        {
            const auto &fst = std::get<ParseSuccess<T>>(a_result);
            const ParseResult<U> b_result = b(fst.rest);
            if (std::holds_alternative<ParseSuccess<U>>(b_result))
            {
                const ParseSuccess<U> &sec = std::get<ParseSuccess<U>>(b_result);
                const Tuple<T, U> value = std::make_tuple(fst.value, sec.value);
                return ParseResult<Tuple<T, U>>(ParseSuccess{value, sec.rest});
            }
            else
            {
                return ParseResult<Tuple<T, U>>(ParseFailure{"Unable to parse tuple (1)"});
            }
        }
        else
        {
            return ParseResult<Tuple<T, U>>(ParseFailure{"Unable to parse tuple (0)"});
        }
    };
}

template <typename T>
Parser<T> OrElse(const Parser<T> &a, const Parser<T> &b)
{
    return [a, b](const std::string_view &input) {
        auto result = a(input);
        if (std::holds_alternative<ParseSuccess<T>>(result))
        {
            return result;
        }
        else
        {
            return ParseResult<T>(b(input));
        }
    };
}

template <typename T>
Parser<T> AnyOf(std::vector<Parser<T>> choices)
{
    return [choices](const std::string_view &input) {
        for (const auto &parser : choices)
        {
            const ParseResult<T> result = parser(input);
            if (std::holds_alternative<ParseSuccess<T>>(result))
            {
                return result;
            }
        }
        return ParseResult<T>(ParseFailure{"Could not parse any choices"});
    };
}

template <typename T, typename U>
Parser<U> Map(const Parser<T> &parser, const std::function<U(T)> &f)
{
    return [parser, f](const std::string_view &input) {
        const ParseResult<T> result = parser(input);
        if (std::holds_alternative<ParseSuccess<T>>(result))
        {
            const auto &succ = std::get<ParseSuccess<T>>(result);
            return ParseResult<U>{ParseSuccess<U>{f(succ.value), succ.rest}};
        }
        else
        {
            return ParseResult<U>(ParseFailure{});
        }
    };
}

Parser<int> Digit()
{
    const std::function<int(char)> char_to_int = [](char c) { return c - '0'; };
    return Map(AnyOf<char>({Literal('1'), Literal('2'), Literal('3'), Literal('4'), Literal('5'),
                            Literal('6'), Literal('7'), Literal('8'), Literal('9'), Literal('0')}),
               char_to_int);
}
