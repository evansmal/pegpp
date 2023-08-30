#include "helper.hpp"

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
    const auto success = std::get<Success>(result);
    throw std::runtime_error("Failed to unwrap parser failure. Remainder: " +
                             success.remainder);
}

auto UnwrapTerminal(const Node &node) -> Terminal
{
    if (std::holds_alternative<Terminal>(node))
    {
        return std::get<Terminal>(node);
    }
    throw std::runtime_error("Failed to unwrap Terminal");
}

auto UnwrapNonTerminal(const Node &node) -> NonTerminal
{
    if (std::holds_alternative<NonTerminal>(node))
    {
        return std::get<NonTerminal>(node);
    }
    throw std::runtime_error("Failed to unwrap NonTerminal");
}
