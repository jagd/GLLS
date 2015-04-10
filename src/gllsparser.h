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
    GllsParser(std::istream &stream_, bool homogeneous = true);
    std::vector<double> run();
    const std::string &xVarName() const { return xVarName_; }
    const SymbolList &symbols() const { return sym_; }
    int xVarSize() const { return xVarSize_; }
    int yVarSize() const { return yVarSize_; }
    const std::vector<double> &coef() const { return coef_; }
    const std::vector<std::pair<int, double> > &xValues() const
        { return xValues_; }
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
    const bool isHomogeneous_;
    int currentLine_;
    int xVarSize_;
    int yVarSize_;
    std::string xVarName_;
    SymbolList sym_;
    std::vector<double> coef_;
    /** the presentation of X_n = c, with n >= 0 in int and c in double */
    std::vector<std::pair<int, double> > xValues_;
    /** list of zerofied polynomials */
    std::vector<std::vector<std::pair<int, double> > > yConds_;
    void solveX(const std::vector<std::vector<std::pair<int, double> > > &);
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_GLLSPARSER_H_
