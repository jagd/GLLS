#include "../parsercommon.h"
#include <streambuf>



#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ParserCommon
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestNextLine)

    BOOST_AUTO_TEST_CASE(NoContent) {
        std::stringstream ss("");
        BOOST_CHECK_EQUAL(nextLine(ss).first, 0);
    }

    BOOST_AUTO_TEST_CASE(SingleLine_1) {
        std::stringstream ss("abc 123");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "abc 123");
    }

    BOOST_AUTO_TEST_CASE(SingleLine_2) {
        std::stringstream ss("abc 123\n");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "abc 123");
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_1) {
        std::stringstream ss("\n");
        BOOST_CHECK_EQUAL(nextLine(ss).first, 0);
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_2) {
        std::stringstream ss("\n \n \n");
        BOOST_CHECK_EQUAL(nextLine(ss).first, 0);
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_3) {
        std::stringstream ss("\n \na\n");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 3);
        BOOST_CHECK_EQUAL(p.second, "a");
    }

    BOOST_AUTO_TEST_CASE(EmptyLine_4) {
        std::stringstream ss("\n a\n");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 2);
        BOOST_CHECK_EQUAL(p.second, " a");
    }

    BOOST_AUTO_TEST_CASE(Comment_1) {
        std::stringstream ss("# abcdefg");
        BOOST_CHECK_EQUAL(nextLine(ss).first, 0);
    }

    BOOST_AUTO_TEST_CASE(Comment_2) {
        std::stringstream ss("123# abcdefg");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "123");
    }

    BOOST_AUTO_TEST_CASE(Comment_3) {
        std::stringstream ss("123 # abcdefg");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 1);
        BOOST_CHECK_EQUAL(p.second, "123");
    }

    BOOST_AUTO_TEST_CASE(Comment_4) {
        std::stringstream ss("\n\n# abcdefg");
        BOOST_CHECK_EQUAL(nextLine(ss).first, 0);
        std::stringstream s2("\n\n # abcdefg\n#def\n\n");
        BOOST_CHECK_EQUAL(nextLine(s2).first, 0);
    }

    BOOST_AUTO_TEST_CASE(Comment_5) {
        std::stringstream ss("\n\n\n#foo\n123");
        auto p = nextLine(ss);
        BOOST_CHECK_EQUAL(p.first, 5);
        BOOST_CHECK_EQUAL(p.second, "123");
    }

BOOST_AUTO_TEST_SUITE_END()
