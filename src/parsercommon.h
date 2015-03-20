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
*           of line consumed is set to zero.
*/
std::pair<int, std::string> nextLine(std::istream &);

class ParserError : public std::exception
{
public:
    ParserError(int line,  const std::string &msg);
    virtual const char *what() const noexcept override;
    int line() const { return line_; }
    const std::string &msg() const { return msg_; }
private:
    const int line_;
    const std::string msg_;
};

#endif //_GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_
