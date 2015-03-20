#include "symbollist.h"
#include <string>
#include <map>
#include <cassert>

bool SymbolList::insert(const std::string &s)
{
    if (s.length() == 0) {
        return false;
    }
    const auto it = map_.find(s);
    if (it == map_.end()) {
        // exception safe order
        map_.insert(std::make_pair(s, nextID_));
        ++nextID_;
        return true;
    }
    return false;
}

int SymbolList::query(const std::string &s) const
{
    const auto it = map_.find(s);
    if (it == map_.end()) {
        return -1;
    }
    return it->second;
}
