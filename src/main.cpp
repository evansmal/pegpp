
#include <iostream>

#include "ccombinators.h"

template <typename T>
void Print(const ParseResult<T> &result)
{
    if (std::holds_alternative<ParseSuccess<T>>(result))
    {
        const auto &succ = std::get<ParseSuccess<T>>(result);
        std::cout << "SUCCESS: " << succ.value << " - " << succ.rest << std::endl;
    }
    else if (std::holds_alternative<ParseFailure>(result))
    {
        const auto &failure = std::get<ParseFailure>(result);
        std::cout << "ERROR: " << failure.message << std::endl;
    }
    else
    {
        throw std::runtime_error("Cannot print parse result");
    }
}

template <typename T, typename U>
void Print(const ParseResult<Tuple<T, U>> &result)
{
    using ReturnType = Tuple<T, U>;
    if (std::holds_alternative<ParseSuccess<Tuple<T, U>>>(result))
    {
        const auto &succ = std::get<ParseSuccess<ReturnType>>(result);
        std::cout << "SUCCESS: (" << std::get<0>(succ.value) << ", " << std::get<1>(succ.value)
                  << ") - " << succ.rest << std::endl;
    }
    else if (std::holds_alternative<ParseFailure>(result))
    {
        const auto &failure = std::get<ParseFailure>(result);
        std::cout << "ERROR: " << failure.message << std::endl;
    }
    else
    {
        throw std::runtime_error("Cannot print parse result");
    }
}

int main()
{
    const auto parser_a = Literal('a');
    const auto parser_b = Literal('b');

    const auto x = parser_a("abc");
    const auto y = parser_a("bcv");

    Print(x);
    Print(y);

    const auto parser_d = OrElse(parser_a, parser_b);

    Print(parser_d("axx"));
    Print(parser_d("bxx"));
    Print(parser_d("xxx"));

    const auto parser_c = AndThen(parser_a, parser_b);

    Print(parser_c("abc"));
    Print(parser_c("bbc"));

    Print(Digit()("123"));
    Print(Digit()("321"));

    const std::function<int(Tuple<char, char>)> f = [](Tuple<char, char> v) {
        const auto str = std::string({std::get<0>(v), std::get<1>(v)});
        return std::stoi(str.c_str());
    };
    const auto seq_digit = Map(AndThen(Literal('1'), Literal('2')), f);
    Print(seq_digit("1234"));

    return 0;
}
