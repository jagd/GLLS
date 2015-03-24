/**

@file condparser.h

========================================

Grammar:

    conds       := expr cond

    cond        := '=' expr cond_tail

    cond_tail   := cond
                 | epsilon

    expr        := term expr_tail

    expr_tail   := '+' expr
                 | '-' expr
                 | epsilon

    term        := atom term_tail

    term_tail   := '*' term
                 | '/' term
                 | epsilon

    atom        := '-' atom_tail
                 | atom_tail

    atom_tail   := id
                 | number
                 | '(' expr ')'

    id          := [a-zA-Z]+ [0-9]+

    number      :=  [+-] [0-9]+ (\.[0-9]+)? ([eE][-+]?[0-9]+)?

========================================

*/

#ifndef _GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_

#include "symbollist.h"
#include <string>
#include <iosfwd>

class CondLexer
{
public:
    CondLexer(std::istream &s, SymbolList, const std::string &varName);
    enum class Token {TK_INVALID, TK_EOF, TK_NUM, TK_ID, TK_OP};
    double num() const { return num_; }
    int symbol() const { return symbol_; }
    const std::string &msg() const { return msg_; }
    Token token();
    //! symbol ID for constant
    static const int ID_CON = -1;
    //! symbol ID for the unknown variable
    static const int ID_VAR = -2;
    //! invalid symbol ID
    static const int ID_INV = -3;
private:
    std::istream &stream_;
    const std::string varName_;
    const SymbolList symList_;
    Token peekMinusOrPlus();
    double num_;
    //! for both ascii symbol and numbered symbol
    int symbol_;
    //! error message
    std::string msg_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_
