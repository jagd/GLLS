/**
* @file GllsParser.h
*/

#ifndef _GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_

#include <iosfwd>

class GllsParser
{
public:
    GllsParser(std::istream &stream_);
private:
    std::istream &stream_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
