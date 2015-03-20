/**
* @file GllsParser.h
*/

#ifndef _GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_

#include "symbollist.h"
#include <iosfwd>
#include <vector>
#include <string>

class GllsParser
{
public:
    GllsParser(std::istream &stream_);
    std::vector<double> run();
private:
    GllsParser(const GllsParser &) = delete;
    GllsParser &operator=(const GllsParser &) = delete;
    std::istream &stream_;
    std::string xName_;
    SymbolList sym_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
