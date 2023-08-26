#include "parser.hpp"

static void Indent(int level)
{
    for (int i = 0; i < level; ++i)
    {
        std::cout << "  ";
    }
}

void Dump(const Node &node, int level)
{
    std::visit(
        [level](auto &&arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Terminal>)
            {
                Indent(level);
                std::cout << "-> Terminal: \"" << arg.value << '"' << std::endl;
            }
            else if constexpr (std::is_same_v<T, NonTerminal>)
            {
                Indent(level);
                std::cout << "NonTerminal: " << arg.type << std::endl;
                for (const auto &child : arg.children)
                {
                    Dump(child, level + 1);
                }
            }
        },
        node);
}
