#pragma once

#include "ast.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Terminal;
struct NonTerminal;

using Node = std::variant<Terminal, NonTerminal>;

void Dump(const Node &node, int level = 0);

struct Terminal
{
    std::string value;
};

struct NonTerminal
{
    std::string type;
    std::vector<Node> children;
};

struct Success
{
    std::vector<Node> node;
    std::string remainder;
};

struct Failure
{
    std::string message;
};

using Result = std::variant<Success, Failure>;

using Parser = std::function<Result(const std::string &)>;
