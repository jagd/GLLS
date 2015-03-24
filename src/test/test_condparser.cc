#include "../condparser.h"
#include "../parsercommon.h"
#include "../symbollist.h"
#include <sstream>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GllsParser
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestCondLexer)

    BOOST_AUTO_TEST_CASE(Ctor_1) {
        std::istringstream ss;
        BOOST_CHECK_NO_THROW(CondLexer lexer(ss, SymbolList(), "x"));
    }

    BOOST_AUTO_TEST_CASE(Ctor_2) {
        std::istringstream ss;
        BOOST_CHECK_THROW(
                CondLexer lexer(ss, SymbolList(), ""),
                std::exception
        );
    }

    BOOST_AUTO_TEST_CASE(TestEOF_1) {
        std::istringstream ss;
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_CHECK(lexer.token() == CondLexer::Token::TK_EOF);
    }

    BOOST_AUTO_TEST_CASE(TestEOF_2) {
        std::istringstream ss(" \t\n\t");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_CHECK(lexer.token() == CondLexer::Token::TK_EOF);
    }

    BOOST_AUTO_TEST_CASE(TestNum_1) {
        std::istringstream ss("1");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), 1.0);
    }

    BOOST_AUTO_TEST_CASE(TestNum_2) {
        std::istringstream ss("-1");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), -1.0);
    }

    BOOST_AUTO_TEST_CASE(TestNum_3) {
        std::istringstream ss("-3.1415926e-1");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -0.31415926, 1e-12);
    }

    BOOST_AUTO_TEST_CASE(TestNum_4) {
        std::istringstream ss("-3.1415926e");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_CHECK(lexer.token() == CondLexer::Token::TK_INVALID);
    }

    BOOST_AUTO_TEST_CASE(TestNum_5) {
        std::istringstream ss("-3.1415926d");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_6) {
        std::istringstream ss("-3.1415926l");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_7) {
        std::istringstream ss("-3.1415926L");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_8) {
        std::istringstream ss("-3.1415926ll");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_9) {
        std::istringstream ss("-3.1415926x");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_10) {
        std::istringstream ss("-3.1415926f");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_11) {
        std::istringstream ss("-3.1415926+");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '+');
    }

    BOOST_AUTO_TEST_CASE(TestNum_12) {
        std::istringstream ss("-3.1415926-");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '-');
    }

    BOOST_AUTO_TEST_CASE(TestNum_13) {
        std::istringstream ss("-3.1415926*");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '*');
    }

    BOOST_AUTO_TEST_CASE(TestNum_14) {
        std::istringstream ss("-3.1415926/");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '/');
    }

    BOOST_AUTO_TEST_CASE(TestOp_1) {
        std::istringstream ss("- 1");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '-');
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), 1.0);
    }

    BOOST_AUTO_TEST_CASE(TestOp_2) {
        std::istringstream ss("- -1");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '-');
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), -1.0);
    }

    BOOST_AUTO_TEST_CASE(TestOp_3) {
        std::istringstream ss("+ +1");
        CondLexer lexer(ss, SymbolList(), "x");
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '+');
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), 1.0);
    }

    BOOST_AUTO_TEST_CASE(TestInvalid) {
        std::istringstream ss("~#'!$%&|^?{}[]`,;:\"\\");
        CondLexer lexer(ss, SymbolList(), "x");
        CondLexer::Token x;
        while ((x = lexer.token()) != CondLexer::Token::TK_EOF) {
            BOOST_CHECK(x == CondLexer::Token::TK_INVALID);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
