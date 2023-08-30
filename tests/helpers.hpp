#pragma once

#include "parser.hpp"

auto UnwrapSuccess(const Result &result) -> Success;

auto UnwrapFailure(const Result &result) -> Failure;

auto UnwrapTerminal(const Node &node) -> Terminal;

auto UnwrapNonTerminal(const Node &node) -> NonTerminal;
