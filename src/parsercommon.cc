#include "parsercommon.h"

#include <istream>
#include <utility>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

// for the compatibility with libstdc++ 4.8, no std::regex is used
std::pair<int, std::string> nextLine(std::istream &stm)
{
    int counter = 0;
    std::string s;
    while (true) {
        if (!stm.good()) {
            counter = 0;
            s.clear();
            break;
        }
        std::getline(stm, s);
        ++counter;
        const auto len = s.find_first_of('#');
        if (len != s.npos) {
            s.resize(len);
        }
        if (std::any_of(s.cbegin(), s.cend(), std::not1(std::cref(isspace)))) {
             break;
        }
    }
    // trim the end
    while (!s.empty() && isspace(*(s.end()-1))) {
        s.pop_back();
    }
    return std::make_pair(counter, std::move(s));
}
