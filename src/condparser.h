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

/**
    @brief Class for the conversion of symbol and ID

    (-inf, ID_VAR_BASE] are used for IDs of unknown variables.
    Whereas [0, inf) are used for IDs of b's from the linear equation Ax=b
*/
class CondDict
{
public:
    CondDict(const SymbolList &, const std::string &varName);
    CondDict(SymbolList &&, const std::string &varName);
    //! symbol ID for constant
    static const int ID_CONST = -1;
    //! invalid symbol ID
    static const int ID_INV = -2;
    //! the first symbol ID for the unknown variable
    static const int ID_VAR_BASE = -10;
    int symToID(const std::string &name, int index) const;
private:
    const std::string varName_;
    const SymbolList symList_;
    void checkVarName() const;
};

class CondLexer
{
public:
    CondLexer(std::istream &, const CondDict &);
    CondLexer(std::istream &, CondDict &&);
    enum class Token {TK_INVALID, TK_EOF, TK_NUM, TK_ID, TK_OP};
    double num() const { return num_; }
    int symbol() const { return symbol_; }
    const std::string &msg() const { return msg_; }
    Token token();
private:
    std::istream &stream_;
    Token peekMinusOrPlus();
    Token peekAlpha();
    double num_;
    /***
        @brief for both ascii symbol and numbered symbol
    */
    int symbol_;
    //! error message
    std::string msg_;
    const CondDict dict_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_
