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

class Lexer
{
public:
    Lexer(std::istream &s, SymbolList, const std::string &varName);
    std::string str;
    double num;
    //! for both ascii symbol and numbered symbol
    int symbol;
    //! symbol ID for constant
    static const int ID_CON = -1;
    //! symbol ID for the unknown variable
    static const int ID_VAR = -2;
    //! invalid symbol ID
    static const int ID_INV = -3;
private:
    std::istream &stream_;
    const std::string varName_;
    const SymbolList sym_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_
