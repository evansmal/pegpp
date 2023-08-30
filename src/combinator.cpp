
#include "combinator.hpp"

#include <stdexcept>

auto Literal(const std::string &value) -> Parser
{
    return [value](const std::string &input)
    {
        for (unsigned int i = 0; i < value.size(); i++)
        {
            if (value[i] != input[i])
            {
                return Result{Failure{"Literal"}};
            }
        }
        Terminal terminal{std::string(value)};
        std::string remainder{input.begin() + static_cast<int>(value.size()),
                              input.end()};
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
        if (c >= start[0] && c <= end[0])
        {
            Terminal terminal{{input.begin(), input.begin() + 1}};
            std::string remainder{input.begin() + 1, input.end()};
            return Result{Success{{terminal}, remainder}};
        }
        return Result{Failure{"Range"}};
    };
}

auto Sequence(const std::vector<Parser> &parsers) -> Parser
{
    if (parsers.empty())
    {
        throw std::runtime_error("Expected sequence to contain at least one parser");
    }
    return [parsers](const std::string &input)
    {
        std::vector<Node> nodes;
        std::string source = input;
        for (const auto &parser : parsers)
        {
            const Result result = parser(source);
            if (std::holds_alternative<Failure>(result))
            {
                return Result{Failure{"Sequence"}};
            }

            const auto &success = std::get<Success>(result);
            for (const auto &node : success.node)
            {
                nodes.push_back(node);
            }
            source = success.remainder;
        }
        return Result{Success{nodes, source}};
    };
}

auto Alternative(const std::vector<Parser> &parsers) -> Parser
{
    if (parsers.empty())
    {
        throw std::runtime_error("Expected alternative to contain at least one parser");
    }
    return [parsers](const std::string &input)
    {
        for (const auto &parser : parsers)
        {
            const Result result = parser(input);
            if (std::holds_alternative<Success>(result))
            {
                const auto &success = std::get<Success>(result);
                return Result{success};
            }
        }
        return Result{Failure{"Alternative"}};
    };
}

auto Optional(const Parser &parser) -> Parser
{
    return [parser](const std::string &input)
    {
        const Result result = parser(input);
        if (std::holds_alternative<Success>(result))
        {
            const auto &success = std::get<Success>(result);
            return Result{success};
        }
        Success success{{}, input};
        return Result{success};
    };
}

auto OneOrMore(const Parser &parser) -> Parser
{
    return [parser](const std::string &input)
    {
        const Result result = parser(input);
        if (std::holds_alternative<Failure>(result))
        {
            return Result{Failure{"Sequence"}};
        }

        const auto &success = std::get<Success>(result);

        std::vector<Node> nodes{success.node};
        std::string source = success.remainder;

        while (true)
        {
            const Result result = parser(source);
            if (std::holds_alternative<Failure>(result))
            {
                return Result{Success{nodes, source}};
            }

            const auto &success = std::get<Success>(result);
            for (const auto &node : success.node)
            {
                nodes.push_back(node);
            }
            source = success.remainder;
        }
    };
}

auto ZeroOrMore(const Parser &parser) -> Parser
{
    return [parser](const std::string &input)
    {
        std::vector<Node> nodes;
        std::string source = input;

        while (true)
        {
            const Result result = parser(source);
            if (std::holds_alternative<Failure>(result))
            {
                return Result{Success{nodes, source}};
            }

            const auto &success = std::get<Success>(result);
            for (const auto &node : success.node)
            {
                nodes.push_back(node);
            }
            source = success.remainder;
        }
    };
}

auto And(const Parser &parser) -> Parser
{
    return [parser](const std::string &input)
    {
        Result result = parser(input);
        if (std::holds_alternative<Success>(result))
        {
            return Result{Success{{}, input}};
        }
        return result;
    };
}

auto Not(const Parser &parser) -> Parser
{
    return [parser](const std::string &input)
    {
        Result result = parser(input);
        if (std::holds_alternative<Success>(result))
        {
            return Result{Failure{"Not"}};
        }
        return Result{Success{{}, input}};
    };
}

auto Dot() -> Parser
{
    return [](const std::string &input)
    {
        if (input.empty())
        {
            return Result{Failure{{"Dot"}}};
        }
        std::string rest{input.begin() + 1, input.end()};
        Terminal terminal{std::string(input.begin(), input.begin() + 1)};
        return Result{Success{{terminal}, rest}};
    };
}

auto Definition(const Parser &parser, const std::string &type) -> Parser
{
    return [parser, type](const std::string &input)
    {
        Result result = parser(input);
        if (std::holds_alternative<Success>(result))
        {
            const auto &success = std::get<Success>(result);
            NonTerminal node{type, success.node};
            return Result{Success{{node}, success.remainder}};
        }
        return result;
    };
}
