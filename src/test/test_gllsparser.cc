#include "../gllsparser.h"
#include <streambuf>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GllsParser
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(ctor) {
        std::stringstream ss;
        BOOST_CHECK_NO_THROW(GllsParser gp(ss));
    }

BOOST_AUTO_TEST_SUITE_END()
