
#include "combinator.hpp"

#include <stdexcept>

auto Literal(const std::string &value) -> Parser
{
    return [value](const std::string &input)
    {
        for (int i = 0; i < value.size(); i++)
        {
            if (value[i] != input[i])
            {
                return Result{Failure{"Literal"}};
            }
        }
        Terminal terminal{std::string(value)};
        std::string remainder{input.begin() + static_cast<int>(value.size()), input.end()};
        return Result{Success{{terminal}, remainder}};
    };
}

auto Range(const std::string &start, const std::string &end) -> Parser
{
    if (start.size() != 1 || end.size() != 1)
    {
        throw std::runtime_error("Expected range bounds to be length=1");
    }
    return [start, end](const std::string &input)
    {
        char c = input[0];
        if (c >= start[0] || c <= end[0])
        {
            Terminal terminal{{input.begin(), input.begin() + 1}};
            std::string remainder{input.begin() + 1, input.end()};
            return Result{Success{{terminal}, remainder}};
        }
        return Result{Failure{"Range"}};
    };
}
