#include "../parsercommon.h"
#include <sstream>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ParserCommon
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestNextLine)

    BOOST_AUTO_TEST_CASE(NoContent) {
        std::istringstream ss("");
        BOOST_CHECK_EQUAL(nextLine(ss).first, -1);
    }

    BOOST_AUTO_TEST_CASE(SingleLine_1) {
        std::istringstream ss("abc 123");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "abc 123");
    }

    BOOST_AUTO_TEST_CASE(SingleLine_2) {
        std::istringstream ss("abc 123\n");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "abc 123");
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_1) {
        std::istringstream ss("\n");
        BOOST_CHECK_EQUAL(nextLine(ss).first, -2);
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_2) {
        std::istringstream ss("\n \n \n");
        BOOST_ASSERT(nextLine(ss).first <= 0);
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_3) {
        std::istringstream ss("\n \na\n");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 3);
        BOOST_CHECK_EQUAL(p.second, "a");
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_4) {
        std::istringstream ss("\n a\n");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 2);
        BOOST_CHECK_EQUAL(p.second, " a");
    }

    BOOST_AUTO_TEST_CASE(Comment_1) {
        std::istringstream ss("# abcdefg");
        BOOST_CHECK_EQUAL(nextLine(ss).first, -1);
    }

    BOOST_AUTO_TEST_CASE(Comment_2) {
        std::istringstream ss("123# abcdefg");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "123");
    }

    BOOST_AUTO_TEST_CASE(Comment_3) {
        std::istringstream ss("123 # abcdefg");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "123");
    }

    BOOST_AUTO_TEST_CASE(Comment_4) {
        std::istringstream ss("\n\n# abcdefg");
        BOOST_CHECK_EQUAL(nextLine(ss).first, -3);
        std::istringstream s2("\n\n # abcdefg\n#def\n\n");
        BOOST_ASSERT(nextLine(s2).first <= 0);
    }

    BOOST_AUTO_TEST_CASE(Comment_5) {
        std::istringstream ss("\n\n\n#foo\n123");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 5);
        BOOST_CHECK_EQUAL(p.second, "123");
    }

BOOST_AUTO_TEST_SUITE_END()
