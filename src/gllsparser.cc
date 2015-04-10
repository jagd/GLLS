#include "gllsparser.h"
#include "condtree.h"
#include "parsercommon.h"
#include "condparser.h"

#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cctype>

GllsParser::GllsParser(std::istream &stream_, bool homo)
        : stream_(stream_), isHomogeneous_(homo), currentLine_(1), xVarSize_(0)
{
}

std::vector<double> GllsParser::run()
{
    readXVarName();
    sym_.clear();
    readYVarNames();
    coef_.clear();
    readCoefWithCond();
    return std::vector<double>();
}

void GllsParser::readXVarName()
{
    static const std::string fail_msg("failed to read name of the unknown");
    const auto l = nextLine(stream_);
    checkGood(l, fail_msg + ": unexpected file end");
    currentLine_ += l.first;
    std::istringstream ss(std::move(l.second));
    if (!(ss >> xVarName_)) {
        throw ParserError(
                currentLine_-1, fail_msg + ": expect a name",
                ParserError::Type::EXPECT_CHAR
        );
    }
    if ( std::any_of(
            xVarName_.cbegin(), xVarName_.cend(),
            std::not1(std::ptr_fun<int,int>(std::isalpha))) )
    {
        throw ParserError(
                currentLine_-1,
                fail_msg + ": invalid name " + xVarName_,
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

void GllsParser::readYVarNames()
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
    std::string firstCond;
    while (true) {
        const auto p = nextLine(stream_);
        checkGood(p, "unexpected file end");
        currentLine_ += p.first;
        const std::string &s = p.second;
        if (s.find('=') != s.npos) {
            firstCond = s;
            break;
        }
        attachCoef(s);
    }
    yVarSize_ = coef_.size() / (xVarSize_+1);
    if ( coef_.size() % ((xVarSize_+1) *sym_.size()) ) {
        throw ParserError(
                currentLine_-1,
                "rows of coefficients are unaligned",
                ParserError::Type::EXPECT_DIGIT
        );
    }
    attachCond(firstCond);
    while (true) {
        const auto p = nextLine(stream_);
        currentLine_ += p.first;
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
        guessXVarSize(s);
        return;
    }
    std::istringstream ss(s);
    const int len = isHomogeneous_ ? xVarSize_ : (xVarSize_+1);
    for (int i = 0; i < len; ++i) {
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
    if (isHomogeneous_) {
        coef_.push_back(0.0);
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

void GllsParser::guessXVarSize(const std::string &s)
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
    if (isHomogeneous_) {
        coef_.push_back(0.0);
    }
    xVarSize_ = static_cast<int>(coef_.size())-1;
    assert(xVarSize_ > 0);
}

static std::list<std::vector<std::pair<int, double> > >
auxLinearEquation(
        const std::string &s,
        const SymbolList &sl,
        const std::string &xName
)
{
    std::istringstream ss(s);
    auto cp = CondParser(ss, sl, xName);
    auto cs = cp.parse();
    std::list<std::vector<std::pair<int, double> > > ls(cs.size());
    std::transform(cs.begin(), cs.end(), ls.begin(),
        [](CondTree &t){
            const auto res = finalizeTree(t);
            if (res != FinalizationStatus::SUCCESS) {
                throw ParserError(
                        0,
                        toString(res),
                        ParserError::Type::SEMANTIC_ERROR
                );
            }
            return toList(t);
        }
    );
    return ls;
}

static bool auxHasSymbol(
        const std::list<std::vector<std::pair<int, double> > > &ls,
        std::function<bool(int)> f
)
{
    return std::any_of(ls.cbegin(), ls.cend(),
            [&f](const std::vector<std::pair<int, double> > &xs) {
                return std::any_of(xs.begin(), xs.end(),
                        [&f](const std::pair<int, double> &x){
                            return f(x.first);
                        }
                );
            }
    );
}

void GllsParser::attachCond(const std::string &s)
{
    assert(!s.empty());
    std::list<std::vector<std::pair<int, double> > > ls;
    try {
        ls = auxLinearEquation(s, sym_, xVarName_);
    } catch (ParserError &e) {
        throw ParserError(e.line()+currentLine_-1, e.msg(), e.type());
    }
    const bool hasX = auxHasSymbol(ls, std::bind2nd(std::less_equal<int>(),
                    static_cast<int>(CondDict::ID_X_VAR_NEG_BASE)));
    const bool hasY = auxHasSymbol(ls, std::bind2nd(std::greater_equal<int>(),
                    static_cast<int>(CondDict::ID_Y_VAR_POS_BASE)));
    if (hasX && hasY) {
        throw ParserError(
                currentLine_-1,
                "can not mix X and Y parameters",
                ParserError::Type::SEMANTIC_ERROR
        );
    }
    if (hasX) {
        solveX(ls);
        return;
    }
    if (hasY) {
        const auto invalidY = auxHasSymbol(ls,
                std::bind2nd(std::greater_equal<int>(), yVarSize_));
        if (invalidY) {
            throw ParserError(
                    currentLine_-1,
                    "too large index of the unknown",
                    ParserError::Type::SEMANTIC_ERROR
            );
        }
        yConds_.insert(yConds_.end(), ls.begin(), ls.end());
        return;
    }
    throw ParserError(
            currentLine_-1,
            "equation does not have any unknown",
            ParserError::Type::SEMANTIC_ERROR
    );
}

void GllsParser::solveX(
        const std::list<std::vector<std::pair<int, double>>>  &ls
)
{
    if (ls.size() != 1) {
        throw ParserError(
                currentLine_-1,
                "this version do not accept multiple equation of variable "
                + xVarName_ + " in one same line",
                ParserError::Type::SEMANTIC_ERROR
        );
    }
    /* solve X */
    if (ls.cbegin()->size() != 2
        || (*ls.cbegin())[1].first != CondDict::ID_CONST) {
        throw ParserError(
                currentLine_-1,
                "this version only solves one "
                + xVarName_ + " variable in one equation",
                ParserError::Type::SEMANTIC_ERROR
        );
    }
    const auto id = CondDict::ID_X_VAR_NEG_BASE-(*ls.cbegin())[0].first;
    if (id >= xVarSize_) {
        throw ParserError(
                currentLine_-1,
                "variable "
                + xVarName_ + std::to_string(id) + " does not exist",
                ParserError::Type::SEMANTIC_ERROR
        );
    }
    xValues_.push_back(std::make_pair(
            id,-(*ls.cbegin())[1].second/(*ls.cbegin())[0].second) );
}
