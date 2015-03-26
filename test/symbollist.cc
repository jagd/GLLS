#include "../src/symbollist.h"

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SymbolList
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(ctor) {
        BOOST_CHECK_NO_THROW(SymbolList());
    }

    BOOST_AUTO_TEST_CASE(EmptySymbol) {
        SymbolList l;
        BOOST_ASSERT(!l.insert(""));
        BOOST_ASSERT(l.query("") < 0);
    }

    BOOST_AUTO_TEST_CASE(Duplication) {
        SymbolList l;
        BOOST_ASSERT(l.insert("123"));
        BOOST_ASSERT(l.insert("abc"));
        BOOST_ASSERT(!l.insert("abc"));
        BOOST_ASSERT(l.insert("def"));
        BOOST_ASSERT(l.query("def") == 2);
    }

    BOOST_AUTO_TEST_CASE(GeneralTest) {
        SymbolList l;
        BOOST_ASSERT(!l.insert(""));
        BOOST_ASSERT(l.insert("abc"));
        BOOST_ASSERT(!l.insert(""));
        BOOST_ASSERT(l.insert("d"));
        BOOST_ASSERT(l.insert("e"));
        BOOST_ASSERT(l.insert("!"));
        BOOST_CHECK_EQUAL(l.query("abc"), 0);
        BOOST_CHECK_EQUAL(l.query("!"), 3);
    }

    BOOST_AUTO_TEST_CASE(CaseSensitive) {
        SymbolList l;
        BOOST_ASSERT(!l.insert(""));
        BOOST_ASSERT(l.insert("abc"));
        BOOST_ASSERT(!l.insert(""));
        BOOST_ASSERT(l.insert("d"));
        BOOST_ASSERT(l.insert("aBc"));
        BOOST_ASSERT(l.insert("!"));
        BOOST_ASSERT(l.query("abC") < 0);
        BOOST_CHECK_EQUAL(l.query("aBc"), 2);
    }

    BOOST_AUTO_TEST_CASE(Clear) {
        SymbolList l;
        BOOST_REQUIRE(l.insert("012"));
        BOOST_REQUIRE(l.insert("abc"));
        BOOST_REQUIRE(l.insert("def"));
        BOOST_CHECK_EQUAL(l.query("abc"), 1);
        l.clear();
        BOOST_REQUIRE(l.insert("abc"));
        BOOST_CHECK_EQUAL(l.query("abc"), 0);
    }

BOOST_AUTO_TEST_SUITE_END()
