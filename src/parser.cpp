#include "parser.hpp"

void DumpTerminal(const Terminal &node)
{
    std::cout << "[ " << node.value << " ]" << std::endl;
}

void DumpNonTerminal(const NonTerminal &node)
{
    std::cout << " >>> " << node.type << std::endl;
    for (const auto &child : node.children)
    {
        Dump(child);
    }
    std::cout << " >>> " << node.type << std::endl;
}

void Dump(const Node &node)
{
    if (std::holds_alternative<Terminal>(node))
    {
        DumpTerminal(std::get<Terminal>(node));
    }
    else if (std::holds_alternative<NonTerminal>(node))
    {
        DumpNonTerminal(std::get<NonTerminal>(node));
    }
    else
    {
        throw std::runtime_error("Invalid node type encountered");
    }
}
