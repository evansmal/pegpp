
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
    return 0;
}
