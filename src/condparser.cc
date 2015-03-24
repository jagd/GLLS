#include "condparser.h"
#include "symbollist.h"
#include "parsercommon.h"

#include <istream>
#include <cassert>
#include <cctype>

CondLexer::CondLexer(
        std::istream &s,
        SymbolList symList,
        const std::string &varName
) : stream_(s), varName_(varName), symList_(symList)
{
    // assert(!varName.empty());
    if (varName.empty()) {
        throw std::logic_error("CondLexer: variable name can not be empty");
    }
}

CondLexer::Token CondLexer::token()
{
    stream_ >> std::ws;
    int peek = stream_.peek();
    if (peek == EOF) {
        return Token::TK_EOF;
    } else if (peek == '-' || peek == '+') {
        return peekMinusOrPlus();
    } else if (peek == '*' || peek == '/') {
        symbol_ = stream_.get();
        return Token::TK_OP;
    } else if (std::isdigit(peek)) {
        stream_ >> num_;
        return Token::TK_NUM;
    }
    symbol_ = stream_.get();
    return Token::TK_INVALID;
}

CondLexer::Token CondLexer::peekMinusOrPlus()
{
    const int ch = stream_.get();
    if (std::isdigit(stream_.peek())){
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
