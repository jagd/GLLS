#include "gllsparser.h"
#include "parsercommon.h"

#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cassert>

GllsParser::GllsParser(std::istream &stream_)
        : stream_(stream_), currentLine_(1), unknownNumber_(0)
{
}

std::vector<double> GllsParser::run()
{
    readUnknownName();
    sym_.clear();
    readSymbols();
    coef_.clear();
    readCoefWithCond();
    return std::vector<double>();
}

void GllsParser::readUnknownName()
{
    static const std::string fail_msg("failed to read name of the unknown");
    const auto l = nextLine(stream_);
    checkGood(l, fail_msg + ": unexpected file end");
    currentLine_ += l.first;
    std::istringstream ss(std::move(l.second));
    if (!(ss >> unknownName_)) {
        throw ParserError(
                currentLine_-1, fail_msg + ": expect a name",
                ParserError::Type::EXPECT_CHAR
        );
    }
    if ( std::any_of(unknownName_.cbegin(), unknownName_.cend(),
            std::not1(std::ptr_fun<int,int>(std::isalpha))) )
    {
        throw ParserError(
                currentLine_-1,
                fail_msg + ": invalid name " + unknownName_,
                ParserError::Type::INVALID_TOKEN
        );

    }
    std::string rest;
    if (ss >> rest) {
        throw ParserError(
                currentLine_-1, fail_msg + ": unexpected content " + rest,
                ParserError::Type::UNEXPECTED_CHAR
        );
    }
}

void GllsParser::checkGood(
        const std::pair<int, std::string> &p,
        const std::string &msg
) const
{
    if (p.first <= 0) {
        throw ParserError(
                currentLine_ - p.first - 1,
                msg,
                ParserError::Type::UNEXPECTED_EOF
        );
    }
}

void GllsParser::readSymbols()
{
    sym_.clear();
    static const std::string fail_msg("failed to read name of symbols");
    const auto l = nextLine(stream_);
    checkGood(l, fail_msg + ": unexpected file end");
    currentLine_ += l.first;
    std::istringstream ss(std::move(l.second));
    std::string s;
    while (ss >> s) {
        if ( std::any_of(s.cbegin(), s.cend(),
                std::not1(std::ptr_fun<int,int>(std::isalpha))) )
        {
            throw ParserError(
                    currentLine_-1,
                    fail_msg + ": invalid name " + s,
                    ParserError::Type::INVALID_TOKEN
            );
        }
        if (!sym_.insert(s)) {
            throw ParserError(
                    currentLine_-1,
                    fail_msg + ": duplicated name " + s,
                    ParserError::Type::UNEXPECTED_CHAR
            );
        }
    }
}

void GllsParser::readCoefWithCond()
{
    while (true) {
        const auto p = nextLine(stream_);
        checkGood(p, "unexpected file end");
        currentLine_ += p.first;
        const std::string &s = p.second;
        if (s.find('=') != s.npos) {
            attachCond(s);
            break;
        }
        attachCoef(s);
    }
    if ( coef_.size() % (unknownNumber_*sym_.size()) ) {
        throw ParserError(
                currentLine_-1,
                "rows of coefficients are unaligned",
                ParserError::Type::EXPECT_DIGIT
        );
    }
    while (true) {
        const auto p = nextLine(stream_);
        if (p.first > 0) {
            attachCond(p.second);
        } else {
            break;
        }
    }
}

void GllsParser::attachCoef(const std::string &s)
{
    if (coef_.size() == 0) {
        estimateUnknownNumber(s);
        return;
    }
    std::istringstream ss(s);
    for (int i = 0; i < unknownNumber_; ++i) {
        double v;
        if (!(ss >> v)) {
            throw ParserError(
                    currentLine_-1,
                    "not enough coefficients on this row",
                    ParserError::Type::EXPECT_DIGIT
            );
        }
        coef_.push_back(v);
    }
    ss >> std::ws;
    if (!ss.eof()) {
        std::string t;
        ss >> t;
        throw ParserError(
                currentLine_-1,
                "invalid content " + t,
                ParserError::Type::UNEXPECTED_CHAR
        );
    }
}

void GllsParser::estimateUnknownNumber(const std::string &s)
{
    std::istringstream ss(s);
    std::string t;
    while (ss >> t) {
        std::istringstream vs(t);
        double v;
        if (!(vs >> v)) {
            throw ParserError(
                    currentLine_-1,
                    "failed to read coefficients: invalid content " + t,
                    ParserError::Type::UNEXPECTED_CHAR
            );
        }
        coef_.push_back(v);
    }
    unknownNumber_ = static_cast<int>(coef_.size());
    assert(unknownNumber_ != 0);
}

void GllsParser::attachCond(const std::string &s)
{
}
