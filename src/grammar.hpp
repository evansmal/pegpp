#pragma once

#include "ast.hpp"

const auto GRAMMAR = ast::Grammar(
    ast::Definition(ast::Identifier("Grammar"),
                    ast::Sequence(ast::Identifier("Spacing"),
                                  ast::OneOrMore(ast::Identifier("Definition")),
                                  ast::Identifier("EndOfFile"))),

    ast::Definition(ast::Identifier("Definition"),
                    ast::Sequence(ast::Identifier("Identifier"),
                                  ast::Identifier("LEFT_ARROW"),
                                  ast::Identifier("Expression"))),

    ast::Definition(
        ast::Identifier("Expression"),
        ast::Sequence(ast::Identifier("Sequence"),
                      ast::ZeroOrMore(ast::Sequence(ast::Identifier("SLASH"),
                                                    ast::Identifier("Sequence"))))),

    ast::Definition(ast::Identifier("Sequence"),
                    ast::ZeroOrMore(ast::Identifier("Prefix"))),

    ast::Definition(ast::Identifier("Prefix"),
                    ast::Sequence(ast::Optional(ast::Alternative(ast::Identifier("AND"),
                                                                 ast::Identifier("NOT"))),
                                  ast::Identifier("Suffix"))),

    ast::Definition(
        ast::Identifier("Suffix"),
        ast::Sequence(ast::Identifier("Primary"),
                      ast::Optional(ast::Alternative(ast::Identifier("QUESTION"),
                                                     ast::Identifier("STAR"),
                                                     ast::Identifier("PLUS"))))),

    ast::Definition(
        ast::Identifier("Primary"),
        ast::Alternative(ast::Sequence(ast::Identifier("Identifier"),
                                       ast::Not(ast::Identifier("LEFT_ARROW"))),
                         ast::Sequence(ast::Identifier("OPEN"),
                                       ast::Identifier("Expression"),
                                       ast::Identifier("CLOSE")),
                         ast::Identifier("Literal"),
                         ast::Identifier("Class"),
                         ast::Identifier("DOT"))),

    ast::Definition(ast::Identifier("Identifier"),
                    ast::Sequence(ast::Identifier("IdentStart"),
                                  ast::ZeroOrMore(ast::Identifier("IdentCont")),
                                  ast::Identifier("Spacing"))),

    ast::Definition(
        ast::Identifier("IdentStart"),
        ast::Class({ast::Range("a", "z"), ast::Range("A", "Z")}, {ast::Literal("_")})),

    ast::Definition(ast::Identifier("IdentCont"),
                    ast::Alternative(ast::Identifier("IdentStart"),
                                     ast::Class({ast::Range("0", "9")}, {}))),

    ast::Definition(
        ast::Identifier("Literal"),
        ast::Alternative(ast::Sequence(ast::Class({}, {ast::Literal("'")}),
                                       ast::ZeroOrMore(ast::Sequence(
                                           ast::Not(ast::Class({}, {ast::Literal("'")})),
                                           ast::Identifier("Char"))),
                                       ast::Class({}, {ast::Literal("'")}),
                                       ast::Identifier("Spacing")),
                         ast::Sequence(ast::Class({}, {ast::Literal("'")}),
                                       ast::ZeroOrMore(ast::Sequence(
                                           ast::Not(ast::Class({}, {ast::Literal("\"")})),
                                           ast::Identifier("Char"))),
                                       ast::Class({}, {ast::Literal("\"")}),
                                       ast::Identifier("Spacing")))),

    ast::Definition(
        ast::Identifier("Class"),
        ast::Sequence(ast::Literal("["),
                      ast::ZeroOrMore(ast::Sequence(ast::Not(ast::Literal("]")),
                                                    ast::Identifier("Range"))),
                      ast::Literal("]"),
                      ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("Range"),
                    ast::Alternative(ast::Sequence(ast::Identifier("Char"),
                                                   ast::Literal("-"),
                                                   ast::Identifier("Char")),
                                     ast::Identifier("Char"))),
    ast::Definition(
        ast::Identifier("Char"),
        ast::Alternative(
            ast::Sequence(ast::Literal("\\"),
                          ast::Class({},
                                     {ast::Literal("n"),
                                      ast::Literal("r"),
                                      ast::Literal("t"),
                                      ast::Literal("'"),
                                      ast::Literal("\""),
                                      ast::Literal("["),
                                      ast::Literal("]"),
                                      ast::Literal("\\")})),
            ast::Sequence(ast::Literal("\\"),
                          ast::Class({ast::Range("0", "2")}, {}),
                          ast::Class({ast::Range("0", "7")}, {}),
                          ast::Class({ast::Range("0", "7")}, {})),
            ast::Sequence(ast::Literal("\\"),
                          ast::Class({ast::Range("0", "7")}, {}),
                          ast::Optional(ast::Class({ast::Range("0", "7")}, {}))),
            ast::Sequence(ast::Literal("\\"), ast::Literal("-")),
            ast::Sequence(ast::Not(ast::Literal("\\")), ast::Dot()))),

    ast::Definition(ast::Identifier("LEFT_ARROW"),
                    ast::Sequence(ast::Literal("<-"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("SLASH"),
                    ast::Sequence(ast::Literal("/"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("AND"),
                    ast::Sequence(ast::Literal("&"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("NOT"),
                    ast::Sequence(ast::Literal("!"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("QUESTION"),
                    ast::Sequence(ast::Literal("?"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("STAR"),
                    ast::Sequence(ast::Literal("*"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("PLUS"),
                    ast::Sequence(ast::Literal("+"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("OPEN"),
                    ast::Sequence(ast::Literal("("), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("CLOSE"),
                    ast::Sequence(ast::Literal(")"), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("DOT"),
                    ast::Sequence(ast::Literal("."), ast::Identifier("Spacing"))),

    ast::Definition(ast::Identifier("Spacing"),
                    ast::ZeroOrMore(ast::Alternative(ast::Identifier("Space"),
                                                     ast::Identifier("Comment")))),
    ast::Definition(ast::Identifier("Comment"),
                    ast::Sequence(ast::Literal("#"),
                                  ast::ZeroOrMore(ast::Sequence(
                                      ast::Not(ast::Identifier("EndOfLine")),
                                      ast::Dot())))),
    ast::Definition(ast::Identifier("Space"),
                    ast::Alternative(ast::Literal(" "),
                                     ast::Literal("\t"),
                                     ast::Identifier("EndOfLine"))),
    ast::Definition(
        ast::Identifier("EndOfLine"),
        ast::Alternative(ast::Literal("\r\n"), ast::Literal("\n"), ast::Literal("\r"))),

    ast::Definition(ast::Identifier("EndOfFile"), ast::Not(ast::Dot())));
