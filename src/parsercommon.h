/**
*   @file parsercommon.h
*/
#ifndef _GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_

#include <iosfwd>
#include <utility>
#include <string>

/**
*   @brief read the next effective line from the stream, after filtering the
*          empty lines and comments
*
*   @return std::pair<line consumed, trimmed string of the effective line>
*           if EOF is reached,  or stream is no more readable , the value
*           of line consumed is set to zero.
*/
std::pair<int, std::string> nextLine(std::istream &);

#endif //_GENERAL_LINEAR_LEAST_SQUARES_PARSERCOMMON_H_
