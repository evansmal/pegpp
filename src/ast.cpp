
#include "ast.hpp"

#include <sstream>
#include <variant>

namespace ast
{

auto ToString(const Literal &literal) -> std::string { return "'" + literal.value + "'"; }

auto ToString(const Range &range) -> std::string
{
    return "[" + range.start + "-" + range.end + "]";
}

auto ToString(const Class &cls) -> std::string
{
    std::stringstream result;
    if (!cls.ranges.empty())
    {
        result << ToString(cls.ranges[0]);
        for (unsigned int i = 1; i < cls.ranges.size(); i++)
        {
            result << " / " << ToString(cls.ranges[i]);
        }
        if (!cls.literals.empty())
        {
            result << " / ";
        }
    }
    if (!cls.literals.empty())
    {
        result << ToString(cls.literals[0]);
        for (unsigned int i = 1; i < cls.literals.size(); i++)
        {
            result << " / " << ToString(cls.literals[i]);
        }
    }
    return result.str();
}

auto ToString(const Expression &expression) -> std::string
{
    std::stringstream result;
    std::visit(
        [&](auto &&expression)
        {
            using T = std::decay_t<decltype(expression)>;
            if constexpr (std::is_same_v<T, Box<ast::Sequence>>)
            {
                if (expression->children.empty())
                {
                    return;
                }
                for (const auto &child : expression->children)
                {
                    result << ToString(child) << " ";
                }
            }
            else if constexpr (std::is_same_v<T, Box<ast::Optional>>)
            {
                result << "(" << ToString(expression->child) << ")?";
            }
            else if constexpr (std::is_same_v<T, Box<ast::ZeroOrMore>>)
            {
                result << "(" << ToString(expression->child) << ")*";
            }
            else if constexpr (std::is_same_v<T, Box<ast::OneOrMore>>)
            {
                result << "" << ToString(expression->child) << "+";
            }
            else if constexpr (std::is_same_v<T, Box<ast::And>>)
            {
                result << "&" << ToString(expression->child);
            }
            else if constexpr (std::is_same_v<T, Box<ast::Not>>)
            {
                result << "!" << ToString(expression->child);
            }
            else if constexpr (std::is_same_v<T, Box<ast::Alternative>>)
            {
                if (expression->children.empty())
                {
                    return;
                }
                result << ToString(expression->children[0]);
                for (unsigned int i = 1; i < expression->children.size(); i++)
                {
                    result << " / " << ToString(expression->children[i]);
                }
            }
            else if constexpr (std::is_same_v<T, ast::Dot>)
            {
                result << ".";
            }
            else if constexpr (std::is_same_v<T, ast::Identifier>)
            {
                result << expression.value;
            }
            else if constexpr (std::is_same_v<T, ast::Class> ||
                               std::is_same_v<T, ast::Literal> ||
                               std::is_same_v<T, ast::Range>)
            {
                result << ToString(expression);
            }
            else
            {
                result << "UNKNOWN";
            }
        },
        expression);
    return result.str();
}

auto ToString(const Definition &definition) -> std::string
{
    std::stringstream result;
    result << definition.identifier.value << " <- " << ToString(definition.expression);
    return result.str();
}

auto ToString(const Grammar &grammar) -> std::string
{
    std::stringstream result;
    for (const auto &definition : grammar.definitions)
    {
        result << ToString(definition) << "\n";
    }
    return result.str();
}

} // namespace ast
