/**

@file condparser.h

========================================

Grammar:

    cond        := expr cond_middle

    cond_middle := '=' expr cond_middle

    cond_tail   := cond_middle
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
#include "condtree.h"
#include <string>
#include <vector>
#include <memory>
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
    static constexpr int ID_CONST = -1;
    //! invalid symbol ID
    static constexpr int ID_INV = -2;
    //! the first symbol ID for the unknown variable
    static constexpr int ID_VAR_BASE = -10;
    //! the first ID for the Y symbols
    static constexpr int ID_SYM_BASE = 0;
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

class CondParser {
public:
    CondParser(std::istream &, const SymbolList &, const std::string &varName);
    /** brief the main parse function */
    std::vector<CondTree> parse();
private:
    CondLexer lexer_;
    CondLexer::Token forward_;
    std::vector<CondTree> parseCond();
    std::vector<CondTree> parseCondMiddle();
    std::unique_ptr<CondTreeNode> parseExpr();
    std::unique_ptr<CondTreeNode> parseTerm();
    std::unique_ptr<CondTreeNode> parseAtom();
    std::unique_ptr<CondTreeNode> parseAtomTail();
};

#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_
