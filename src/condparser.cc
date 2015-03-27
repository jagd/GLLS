#include "condparser.h"
#include "symbollist.h"
#include "parsercommon.h"

#include <istream>
#include <sstream>
#include <cassert>
#include <cctype>

//constexpr int CondDict::ID_CONST;
//constexpr int CondDict::ID_INV;
//constexpr int CondDict::ID_VAR_BASE;
//constexpr int CondDict::ID_SYM_BASE;

CondDict::CondDict(const SymbolList &sl, const std::string &varName)
    : varName_(varName), symList_(sl)
{
    // assert(!varName_.empty());
    checkVarName();
}

CondDict::CondDict(SymbolList &&sl, const std::string &varName)
    : varName_(varName), symList_(sl)
{
    // assert(!varName_.empty());
    checkVarName();
}

void CondDict::checkVarName() const
{
    if (varName_.empty()) {
        throw std::logic_error("CondLexer: variable name can not be empty");
    }
}

int CondDict::symToID(const std::string &name, int index) const
{
    assert(index >= 0);
    if (name == varName_) {
        return ID_VAR_BASE - index;
    }
    const int offset = symList_.query(name);
    if (offset >= 0) {
        return static_cast<int>(symList_.size() * index + offset);
    }
    return ID_INV;
}

CondLexer::CondLexer(std::istream &s, const CondDict &d)
        : stream_(s), dict_(d)
{
}

CondLexer::CondLexer(std::istream &s, CondDict &&d)
        : stream_(s), dict_(d)
{
}

CondLexer::Token CondLexer::token()
{
    stream_ >> std::ws;
    const int peek = static_cast<int>(stream_.peek());
    if (peek == std::istream::traits_type::eof()) {
        return Token::TK_EOF;
    } else if (std::isalpha(peek)) {
        return peekAlpha();
    } else if (std::isdigit(peek)) {
        stream_ >> num_;
        return Token::TK_NUM;
    } else switch (peek) {
        case '-':
        case '+':
            return peekMinusOrPlus();
        case '*':
        case '/':
        case '(':
        case ')':
        case '=':
            symbol_ = static_cast<int>(stream_.get());
            return Token::TK_OP;
        default:
            break;
    }
    symbol_ = static_cast<int>(stream_.get());
    msg_ = "invalid symbol ";
    msg_ += static_cast<char>(symbol_);
    return Token::TK_INVALID;
}

CondLexer::Token CondLexer::peekMinusOrPlus()
{
    const int ch = static_cast<int>(stream_.get());
    if (std::isdigit(static_cast<int>(stream_.peek()))){
        if (!(stream_ >> num_)) {
            msg_ = std::string("not a valid number after '")
                    + static_cast<char>(ch) + "' sign";
            return Token::TK_INVALID;
        }
        if (ch == '-') {
            num_ = -num_;
        }
        return Token::TK_NUM;
    } else {
        symbol_ = ch;
        return Token::TK_OP;
    }
}

CondLexer::Token CondLexer::peekAlpha()
{
    std::string name;
    std::string numstr;
    while (std::isalpha(static_cast<int>(stream_.peek()))) {
        name.push_back(static_cast<char>(stream_.get()));
    }
    while (std::isdigit(static_cast<int>(stream_.peek()))) {
        numstr.push_back(static_cast<char>(stream_.get()));
    }
    if (numstr.empty()) {
        msg_ = name + " should follow an integer index";
        return Token::TK_INVALID;
    }
    // do not use std::istringstream because it differs from 010 and 10
    int num = 0;
    for (const auto i : numstr) {
        num *= 10;
        num += i - '0';
    }
    msg_.clear();
    symbol_ = dict_.symToID(name, num);
    if (symbol_ == dict_.ID_INV)  {
        msg_ += ", invalid symbol ";
        msg_ += name;
        msg_ += numstr;
        return Token::TK_INVALID;
    }
    return Token::TK_ID;
}

CondParser::CondParser(
        std::istream & s,
        const SymbolList &l,
        const std::string &varName
) : lexer_(s, CondDict(l, varName))
{
    forward_ = lexer_.token();
}

//std::vector<std::unique_ptr<CondTreeNode> > CondParser::parse_conds()
//{
//    typedef std::unique_ptr<CondTreeNode> Tree;
//    std::vector<Tree> trees;
//    auto t = parse_expr();
//    if (t) {
//        trees.push_back(t);
//    }
//}
//
//std::unique_ptr<CondTreeNode> CondParser::parse_expr()
//{
//    return std::unique_ptr<CondTreeNode>();
//}
