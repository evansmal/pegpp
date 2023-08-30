#pragma once

#include "ast.hpp"
#include "parser.hpp"

struct Collection
{

  public:
    void Add(const std::string &name, const Parser &parser)
    {
        if (storage.contains(name))
        {
            throw std::runtime_error("Tried to add parser " + name +
                                     " to collection but it already exists");
        }
        storage[name] = parser;
    }

    [[nodiscard]] auto Get(const std::string &name) const -> Parser
    {
        if (storage.contains(name))
        {
            return storage.at(name);
        }
        throw std::runtime_error("Tried to retrieve parser " + name +
                                 " from collection but it doens't exist");
    }

  private:
    std::map<std::string, Parser> storage;
};

auto Generate(const ast::Grammar &grammar) -> Collection;
