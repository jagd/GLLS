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
    const std::string &unknownName() const { return unknownName_; }
    const SymbolList &symbols() const { return sym_; }
private:
    GllsParser(const GllsParser &) = delete;
    GllsParser &operator=(const GllsParser &) = delete;
    void checkGood(
            const std::pair<int, std::string> &p, const std::string &msg
    ) const;
    void readUnknownName();
    void readSymbols();
    std::istream &stream_;
    int currentLine_;
    std::string unknownName_;
    SymbolList sym_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
