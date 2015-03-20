#include "gllsparser.h"
#include <vector>
#include <streambuf>

GllsParser::GllsParser(std::istream &stream_) : stream_(stream_)
{
}

std::vector<double> GllsParser::run()
{
    readUnknownName();
    return std::vector<double>();
}

void GllsParser::readUnknownName()
{
 /// todo
}
