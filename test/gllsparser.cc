#include "../src/gllsparser.h"
#include "../src/parsercommon.h"
#include <sstream>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GllsParser
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(ctor) {
        std::istringstream ss;
        BOOST_CHECK_NO_THROW(GllsParser gp(ss));
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_1) {
        std::istringstream ss("");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );

        try {
            gp.run();
        } catch (ParserError &e) {
            BOOST_CHECK(e.what() != nullptr);
        }
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_2) {
        std::istringstream ss("\n # comment\n");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 3);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_3) {
        std::istringstream ss("   1  c");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::INVALID_TOKEN;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_4) {
        std::istringstream ss("   x  c");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadUnknownName_5) {
        std::istringstream ss("\0\0\0");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 1);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadSymbols_1) {
        std::istringstream ss("x\n");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 2);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadSymbols_2) {
        std::istringstream ss("x\na 1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 2);
                    return e.type() == ParserError::Type::INVALID_TOKEN;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadSymbols_3) {
        std::istringstream ss("x\na b c b");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 2);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadMatrix_1) {
        std::istringstream ss("x\ny\n");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 3);
                    return e.type() == ParserError::Type::UNEXPECTED_EOF;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadMatrix_2) {
        std::istringstream ss("x\ny\n1 2 3 4 a\n x=1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 3);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadMatrix_3) {
        std::istringstream ss("x\ny z\n1 2 3 4 \n x=1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 4);
                    return e.type() == ParserError::Type::EXPECT_DIGIT;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadMatrix_4) {
        std::istringstream ss("x\ny\n1 2 3 4 \n 4 5 6\n x=1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 4);
                    return e.type() == ParserError::Type::EXPECT_DIGIT;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadMatrix_5) {
        std::istringstream ss("x\ny\n1 2 3 4 \n 4 5 6 7 c\n x=1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 4);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

    BOOST_AUTO_TEST_CASE(ReadMatrix_6) {
        std::istringstream ss("x\ny\n1 2 3 4 \n 4 5 6 7 8\n x=1");
        GllsParser gp(ss);
        BOOST_CHECK_EXCEPTION(gp.run(), ParserError,
                [](const ParserError &e) {
                    BOOST_CHECK_EQUAL(e.line(), 4);
                    return e.type() == ParserError::Type::UNEXPECTED_CHAR;
                }
        );
    }

BOOST_AUTO_TEST_SUITE_END()
