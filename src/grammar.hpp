#pragma once

#include "ast.hpp"

auto GetPegGrammarAST() -> ast::Grammar
{
    using namespace ast;
    return {
        Definition(Identifier("Grammar"),
                   Sequence(Identifier("Spacing"),
                            OneOrMore(Identifier("Definition")),
                            Identifier("EndOfFile"))),

        Definition(Identifier("Definition"),
                   Sequence(Identifier("Identifier"),
                            Identifier("LEFT_ARROW"),
                            Identifier("Expression"))),

        Definition(
            Identifier("Expression"),
            Sequence(Identifier("Sequence"),
                     ZeroOrMore(Sequence(Identifier("SLASH"), Identifier("Sequence"))))),

        Definition(Identifier("Sequence"), ZeroOrMore(Identifier("Prefix"))),

        Definition(Identifier("Prefix"),
                   Sequence(Optional(Alternative(Identifier("AND"), Identifier("NOT"))),
                            Identifier("Suffix"))),

        Definition(Identifier("Suffix"),
                   Sequence(Identifier("Primary"),
                            Optional(Alternative(Identifier("QUESTION"),
                                                 Identifier("STAR"),
                                                 Identifier("PLUS"))))),

        Definition(
            Identifier("Primary"),
            Alternative(Sequence(Identifier("Identifier"), Not(Identifier("LEFT_ARROW"))),
                        Sequence(Identifier("OPEN"),
                                 Identifier("Expression"),
                                 Identifier("CLOSE")),
                        Identifier("Literal"),
                        Identifier("Class"),
                        Identifier("DOT"))),

        Definition(Identifier("Identifier"),
                   Sequence(Identifier("IdentStart"),
                            ZeroOrMore(Identifier("IdentCont")),
                            Identifier("Spacing"))),

        Definition(Identifier("IdentStart"),
                   Class({Range("a", "z"), Range("A", "Z")}, {Literal("_")})),

        Definition(Identifier("IdentCont"),
                   Alternative(Identifier("IdentStart"), Class({Range("0", "9")}, {}))),

        Definition(
            Identifier("Literal"),
            Alternative(Sequence(Class({}, {Literal("'")}),
                                 ZeroOrMore(Sequence(Not(Class({}, {Literal("'")})),
                                                     Identifier("Char"))),
                                 Class({}, {Literal("'")}),
                                 Identifier("Spacing")),
                        Sequence(Class({}, {Literal("'")}),
                                 ZeroOrMore(Sequence(Not(Class({}, {Literal("\"")})),
                                                     Identifier("Char"))),
                                 Class({}, {Literal("\"")}),
                                 Identifier("Spacing")))),

        Definition(Identifier("Class"),
                   Sequence(Literal("["),
                            ZeroOrMore(Sequence(Not(Literal("]")), Identifier("Range"))),
                            Literal("]"),
                            Identifier("Spacing"))),

        Definition(
            Identifier("Range"),
            Alternative(Sequence(Identifier("Char"), Literal("-"), Identifier("Char")),
                        Identifier("Char"))),

        Definition(Identifier("Char"),
                   Alternative(Sequence(Literal("\\"),
                                        Class({},
                                              {Literal("n"),
                                               Literal("r"),
                                               Literal("t"),
                                               Literal("'"),
                                               Literal("\""),
                                               Literal("["),
                                               Literal("]"),
                                               Literal("\\")})),
                               Sequence(Literal("\\"),
                                        Class({Range("0", "2")}, {}),
                                        Class({Range("0", "7")}, {}),
                                        Class({Range("0", "7")}, {})),
                               Sequence(Literal("\\"),
                                        Class({Range("0", "7")}, {}),
                                        Optional(Class({Range("0", "7")}, {}))),
                               Sequence(Literal("\\"), Literal("-")),
                               Sequence(Not(Literal("\\")), Dot()))),

        Definition(Identifier("LEFT_ARROW"),
                   Sequence(Literal("<-"), Identifier("Spacing"))),

        Definition(Identifier("SLASH"), Sequence(Literal("/"), Identifier("Spacing"))),

        Definition(Identifier("AND"), Sequence(Literal("&"), Identifier("Spacing"))),

        Definition(Identifier("NOT"), Sequence(Literal("!"), Identifier("Spacing"))),

        Definition(Identifier("QUESTION"), Sequence(Literal("?"), Identifier("Spacing"))),

        Definition(Identifier("STAR"), Sequence(Literal("*"), Identifier("Spacing"))),

        Definition(Identifier("PLUS"), Sequence(Literal("+"), Identifier("Spacing"))),

        Definition(Identifier("OPEN"), Sequence(Literal("("), Identifier("Spacing"))),

        Definition(Identifier("CLOSE"), Sequence(Literal(")"), Identifier("Spacing"))),

        Definition(Identifier("DOT"), Sequence(Literal("."), Identifier("Spacing"))),

        Definition(Identifier("Spacing"),
                   ZeroOrMore(Alternative(Identifier("Space"), Identifier("Comment")))),

        Definition(Identifier("Comment"),
                   Sequence(Literal("#"),
                            ZeroOrMore(Sequence(Not(Identifier("EndOfLine")), Dot())))),

        Definition(Identifier("Space"),
                   Alternative(Literal(" "), Literal("\t"), Identifier("EndOfLine"))),

        Definition(Identifier("EndOfLine"),
                   Alternative(Literal("\r\n"), Literal("\n"), Literal("\r"))),

        Definition(Identifier("EndOfFile"), Not(Dot()))};
}
