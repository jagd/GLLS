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
    void readXVarName();
    void readYVarNames();
    /**
    *   In order to avoid unget() on stream, combine
    *   the reading of coefficients and conditions together
    */
    void readCoefWithCond();
    void attachCoef(const std::string &s);
    void attachCond(const std::string &s);
    void guessXVarSize(const std::string &s);
    std::istream &stream_;
    int currentLine_;
    int xVarSize_;
    std::string unknownName_;
    SymbolList sym_;
    std::vector<double> coef_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
