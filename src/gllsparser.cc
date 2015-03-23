#include "gllsparser.h"
#include "parsercommon.h"

#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>

GllsParser::GllsParser(std::istream &stream_)
        : stream_(stream_), currentLine_(1)
{
}

std::vector<double> GllsParser::run()
{
    readUnknownName();
    return std::vector<double>();
}

void GllsParser::readUnknownName()
{
    static const std::string fail_msg("failed to read name of the unknown");
    const auto l = nextLine(stream_);
    checkGood(l, fail_msg + ": unexpected file end");
    currentLine_ += l.first;
    std::stringstream ss(std::move(l.second));
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
                currentLine_-1, fail_msg + ": unexpected content" + rest,
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
