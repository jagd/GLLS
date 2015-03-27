#include "../src/parsercommon.h"
#include "../src/symbollist.h"
#include "../src/condparser.h"
#include <sstream>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GllsParser
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestCondDict)

    BOOST_AUTO_TEST_CASE(Ctor_1) {
        BOOST_CHECK_NO_THROW(CondDict(SymbolList(), "x"));
    }

    BOOST_AUTO_TEST_CASE(Ctor_2) {
        BOOST_CHECK_THROW(
                CondDict(SymbolList(), ""),
                std::exception
        );
    }

    BOOST_AUTO_TEST_CASE(TestSymToID_X) {
        auto sl = SymbolList();
        sl.insert("y");
        const auto dict = CondDict(sl, "x");
        BOOST_CHECK_EQUAL(
                dict.symToID("", 0),
                static_cast<int>(dict.ID_INV)
        );
        BOOST_CHECK_EQUAL(
                dict.symToID("x", 0),
                static_cast<int>(dict.ID_VAR_BASE)
        );
        BOOST_CHECK_EQUAL(
                dict.symToID("x", 100),
                static_cast<int>(dict.ID_VAR_BASE-100)
        );
    }

    BOOST_AUTO_TEST_CASE(TestSymToID_Y) {
        auto sl = SymbolList();
        sl.insert("bz");
        sl.insert("br");
        sl.insert("phi");
        const auto dict = CondDict(sl, "x");
        BOOST_CHECK_EQUAL(dict.symToID("br", 0), 1);
        BOOST_CHECK_EQUAL(dict.symToID("phi", 2), 8);
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestCondLexer)

    BOOST_AUTO_TEST_CASE(Ctor_1) {
        std::istringstream ss;
        const auto dict = CondDict(SymbolList(), "x");
        BOOST_CHECK_NO_THROW(CondLexer lexer(ss, dict));
    }

    BOOST_AUTO_TEST_CASE(TestEOF_1) {
        std::istringstream ss;
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_CHECK(lexer.token() == CondLexer::Token::TK_EOF);
    }

    BOOST_AUTO_TEST_CASE(TestEOF_2) {
        std::istringstream ss(" \t\n\t");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_CHECK(lexer.token() == CondLexer::Token::TK_EOF);
    }

    BOOST_AUTO_TEST_CASE(TestNum_1) {
        std::istringstream ss("1");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), 1.0);
    }

    BOOST_AUTO_TEST_CASE(TestNum_2) {
        std::istringstream ss("-1");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), -1.0);
    }

    BOOST_AUTO_TEST_CASE(TestNum_3) {
        std::istringstream ss("-3.1415926e-1");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -0.31415926, 1e-12);
    }

    BOOST_AUTO_TEST_CASE(TestNum_4) {
        std::istringstream ss("-3.1415926e");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_CHECK(lexer.token() == CondLexer::Token::TK_INVALID);
    }

    BOOST_AUTO_TEST_CASE(TestNum_5) {
        std::istringstream ss("-3.1415926d");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_6) {
        std::istringstream ss("-3.1415926l");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_7) {
        std::istringstream ss("-3.1415926L");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_8) {
        std::istringstream ss("-3.1415926ll");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_9) {
        std::istringstream ss("-3.1415926x");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_10) {
        std::istringstream ss("-3.1415926f");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
    }

    BOOST_AUTO_TEST_CASE(TestNum_11) {
        std::istringstream ss("-3.1415926+");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '+');
    }

    BOOST_AUTO_TEST_CASE(TestNum_12) {
        std::istringstream ss("-3.1415926-");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '-');
    }

    BOOST_AUTO_TEST_CASE(TestNum_13) {
        std::istringstream ss("-3.1415926*");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '*');
    }

    BOOST_AUTO_TEST_CASE(TestNum_14) {
        std::istringstream ss("-3.1415926/");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_CLOSE(lexer.num(), -3.1415926, 1e-6);
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '/');
    }

    BOOST_AUTO_TEST_CASE(TestOp_1) {
        std::istringstream ss("- 1");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '-');
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), 1.0);
    }

    BOOST_AUTO_TEST_CASE(TestOp_2) {
        std::istringstream ss("- -1");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '-');
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), -1.0);
    }

    BOOST_AUTO_TEST_CASE(TestOp_3) {
        std::istringstream ss("+ +1");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_OP);
        BOOST_CHECK_EQUAL(lexer.symbol(), '+');
        BOOST_REQUIRE(lexer.token() == CondLexer::Token::TK_NUM);
        BOOST_CHECK_EQUAL(lexer.num(), 1.0);
    }

    BOOST_AUTO_TEST_CASE(TestInvalid) {
        std::istringstream ss("~#'!$%&|^?{}[]`,;:\"\\");
        CondLexer lexer(ss, CondDict(SymbolList(), "x"));
        CondLexer::Token x;
        while ((x = lexer.token()) != CondLexer::Token::TK_EOF) {
            BOOST_CHECK(x == CondLexer::Token::TK_INVALID);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
