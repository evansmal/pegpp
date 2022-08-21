#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>

#include <iostream>

template <typename T>
struct ParseSuccess
{
    T value;
    std::string rest;
};

struct ParseFailure
{
    std::string message;
};

template <typename T>
using ParseResult = std::variant<ParseSuccess<T>, ParseFailure>;

template <typename T>
using Parser = std::function<ParseResult<T>(const std::string &)>;

Parser<char> pchar(char c)
{
    return [c](const std::string &input) {
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

template <typename T>
Parser<T> AndThen(const Parser<T> &a, const Parser<T> &b)
{
    return [&a, &b](const std::string &input) {
        const auto result = a(input);
        if (std::holds_alternative<ParseSuccess<T>>(result))
        {
            const auto &succ = std::get<ParseSuccess<T>>(result);
            return b(succ.rest);
        }
        else
        {
            return ParseResult<T>(result);
        }
    };
}

template <typename T>
Parser<T> OrElse(const Parser<T> &a, const Parser<T> &b)
{
    return [&a, &b](const std::string &input) {
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
Parser<T> AnyOf(const std::vector<Parser<T>> &choices)
{
    return [choices](const std::string &input) {
        for (const auto &parser : choices)
        {
            auto result = parser(input);
            if (std::holds_alternative<ParseSuccess<T>>(result))
            {
                return result;
            }
        }
        return ParseResult<T>(ParseFailure{"Could not parse any choices"});
    };
}

template <typename T>
Parser<T> Map(const Parser<T> &parser)
{
    return [&parser](const std::string &input) {};
}

int main()
{
    const auto parser_a = pchar('a');
    const auto parser_b = pchar('b');

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

    const auto parse_digit =
        AnyOf<char>({pchar('1'), pchar('2'), pchar('3'), pchar('4'), pchar('5'), pchar('6'),
                     pchar('7'), pchar('8'), pchar('9'), pchar('0')});

    Print(parse_digit("0A"));
    Print(parse_digit("9A"));

    return 0;
}
