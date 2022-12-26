
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

int main()
{
    const auto parser_a = Literal('a');
    const auto parser_b = Literal('b');

    const auto x = parser_a("abc");
    const auto y = parser_a("bcv");

    Print(x);
    Print(y);

    const auto parser_c = AndThen(parser_a, parser_b);

    Print(parser_c("abc"));
    Print(parser_c("bbc"));

    const auto parser_d = OrElse(parser_a, parser_b);

    Print(parser_d("axx"));
    Print(parser_d("bxx"));
    Print(parser_d("xxx"));

    const auto parser_e = AnyOf<char>({parser_b, parser_c});

    Print(parser_e("bxx"));
    Print(parser_e("axx"));
    Print(parser_e("abx"));
    Print(parser_e("bax"));

    const auto parser_f = AndThen(parser_e, parser_b);

    Print(parser_f("bbx"));

    Print(Digit()("P0"));
    Print(Digit()("0A"));
    Print(Digit()("9A"));

    const std::function<int(char)> ToInt = [](char c) {
        const std::string d = {c};
        return std::stoi(d.c_str());
    };
    const Parser<int> parser_g = Map(Digit(), ToInt);
    Print(Digit()("7"));
    Print(Digit()("1"));
    Print(Digit()("A"));

    return 0;
}
