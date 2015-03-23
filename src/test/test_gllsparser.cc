#include "../gllsparser.h"
#include "../parsercommon.h"
#include <sstream>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GllsParser
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(ctor) {
        std::stringstream ss;
        BOOST_CHECK_NO_THROW(GllsParser gp(ss));
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_1) {
        std::stringstream ss("");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_2) {
        std::stringstream ss("\n # comment\n");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 3);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_3) {
        std::stringstream ss("   1  c");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::INVALID_TOKEN;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_4) {
        std::stringstream ss("   x  c");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadSymbols_1) {
        std::stringstream ss("x\n");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 2);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadSymbols_2) {
        std::stringstream ss("x\na 1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 2);
                    return e.type() == ParserError::Type::INVALID_TOKEN;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadSymbols_3) {
        std::stringstream ss("x\na b c b");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 2);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

BOOST_AUTO_TEST_SUITE_END()
