#include "../src/condtree.h"

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CondTree
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(Ctor) {
        BOOST_CHECK(CondTreeNode().type == CondTreeNode::Type::INVALID_NODE);
        BOOST_CHECK(CondTreeNode('+').type == CondTreeNode::Type::OP_NODE);
        BOOST_CHECK(CondTreeNode(0).type == CondTreeNode::Type::ID_NODE);
        BOOST_CHECK(CondTreeNode(3.14).type == CondTreeNode::Type::NUM_NODE);
    }

    BOOST_AUTO_TEST_CASE(IsTerm_1) {
        const auto root = CondTreeNode();
        BOOST_CHECK_EQUAL(root.isTerm(), true);
    }

BOOST_AUTO_TEST_SUITE_END()
