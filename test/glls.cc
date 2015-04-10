#include "../src/parsercommon.h"
#include "../src/symbollist.h"
#include "../src/condparser.h"
#include "../src/condtree.h"
#include "../src/glls.h"
#include "../src/gllsparser.h"
#include <sstream>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Glls
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(SolveX_1) {
        std::istringstream ss(
            "x\ny\n1 2 3 4 \n 4 5 6 8\n (x0-3)*5=1+1*4"
        );
        GllsParser gp(ss, true);
        GllsProblem g;
        BOOST_REQUIRE_NO_THROW(g = gp.run());
        BOOST_REQUIRE_EQUAL(gp.xVarName(), "x");
        BOOST_REQUIRE_CLOSE(gp.xValues()[0].second, 4.0, 1e-9);
        arrangeX(g, gp.xValues());
        BOOST_CHECK_CLOSE(g.coef[3], 4.0, 1e-9);
        BOOST_CHECK_CLOSE(g.coef[7], 16.0, 1e-9);
    }

    BOOST_AUTO_TEST_CASE(SolveX_2) {
        std::istringstream ss(
                "x\ny\n1 2 3 4 \n 4 5 6 8\n x0=1\n x3 = -2"
        );
        GllsParser gp(ss, true);
        GllsProblem g;
        BOOST_REQUIRE_NO_THROW(g = gp.run());
        arrangeX(g, gp.xValues());
        BOOST_CHECK_CLOSE(g.coef[2], -7.0, 1e-9);
        BOOST_CHECK_CLOSE(g.coef[5], -12.0, 1e-9);
    }

BOOST_AUTO_TEST_SUITE_END()
