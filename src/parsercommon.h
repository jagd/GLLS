/**
*   @file parsercommon.h
*/
#ifndef _GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_

#include <iosfwd>
#include <utility>
#include <string>
#include <stdexcept>

/**
*   @brief read the next effective line from the stream, after filtering the
*          empty lines and comments
*
*   @return std::pair<line consumed, trimmed string of the effective line>
*           if EOF is reached,  or stream is no more readable , the value
*           of line consumed is set to negative number of consumed lines.
*/
std::pair<int, std::string> nextLine(std::istream &);

class ParserError : public std::exception
{
public:
    enum class Type
    {
        UNKNOWN,
        UNEXPECTED_EOF,
        UNEXPECTED_CHAR,
        EXPECT_CHAR,
        EXPECT_DIGIT,
        INVALID_TOKEN,
        SEMANTIC_ERROR,
    };
    ParserError(
            int line,
            const std::string &msg,
            Type t = Type::UNKNOWN
    );
    virtual const char *what() const noexcept override;
    int line() const { return line_; }
    const std::string &msg() const { return msg_; }
    Type type() const { return type_; }
private:
    const int line_;
    const std::string msg_;
    Type type_;
};

#endif //_GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_
