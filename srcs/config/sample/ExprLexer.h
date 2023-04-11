
// Generated from ./srcs/Expr.g4 by ANTLR 4.12.0

#pragma once

#include "antlr4-runtime.h"

class ExprLexer : public antlr4::Lexer {
   public:
    // lexerに沿ってenumを定義する
    enum {
        AND = 1,
        OR = 2,
        NOT = 3,
        EQ = 4,
        COMMA = 5,
        SEMI = 6,
        DQUOTE = 7,
        LCURLY = 8,
        RCURLY = 9,
        INT = 10,
        ID = 11,
        WS = 12,
        ANYCHAR = 13,
        NUMBER = 14
    };

    // antlr4::Lexerのコンストラクタを呼び出す
    explicit ExprLexer(antlr4::CharStream* input);

    ~ExprLexer() override;

    std::string getGrammarFileName() const override;

    const std::vector<std::string>& getRuleNames() const override;

    const std::vector<std::string>& getChannelNames() const override;

    const std::vector<std::string>& getModeNames() const override;

    const antlr4::dfa::Vocabulary& getVocabulary() const override;

    antlr4::atn::SerializedATNView getSerializedATN() const override;

    const antlr4::atn::ATN& getATN() const override;

    // By default the static state used to implement the lexer is lazily
    // initialized during the first call to the constructor. You can call this
    // function if you wish to initialize the static state ahead of time.
    static void initialize();

   private:
    // Individual action functions triggered by action() above.

    // Individual semantic predicate functions triggered by sempred() above.
};
