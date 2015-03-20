/**
*   @file symbollist.h
*/

#ifndef _GENERAL_LINEAR_LEAST_SQUARES_SYMBOLLIST_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_SYMBOLLIST_H_

#include <string>
#include <map>

/**
* @brief A case sensitive symbol list, which count the symbols as their
*        insert order 0, 1, 2, 3 ...
*/
class SymbolList
{
public:
    SymbolList() : nextID_(0) {}
    //! @return true if success
    bool insert(const std::string &);
    //! @return non-negative id if found, otherwise negative
    int query(const std::string &) const;
    std::string query_id(int) const = delete;
private:
    int nextID_;
    std::map<std::string, int> map_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_SYMBOLLIST_H_
