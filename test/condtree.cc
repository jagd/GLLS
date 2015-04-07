#include "../src/condtree.h"
#include "../src/condparser.h"
#include "../src/symbollist.h"
#include "../src/parsercommon.h"
#include <memory>

#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CondTree
#endif
#include <boost/test/unit_test.hpp>
#include <memory>

BOOST_AUTO_TEST_SUITE(TestCondTree)

    BOOST_AUTO_TEST_CASE(Ctor) {
        BOOST_CHECK(CondTreeNode().type == CondTreeNode::Type::INVALID_NODE);
        BOOST_CHECK(CondTreeNode('+').type == CondTreeNode::Type::OP_NODE);
        BOOST_CHECK(CondTreeNode(0).type == CondTreeNode::Type::ID_NODE);
        BOOST_CHECK(CondTreeNode(3.14).type == CondTreeNode::Type::NUM_NODE);
    }

    BOOST_AUTO_TEST_CASE(CopyCtor) {
        auto root = CondTreeNode::make('*');
        root->left = CondTreeNode::make(1);
        root->right = CondTreeNode::make(2);
        auto c = root->clone();
        root->left = CondTreeNode::make('-');
        BOOST_CHECK(!root->isValid());
        BOOST_CHECK(c->isValid());
    }

    BOOST_AUTO_TEST_CASE(CtorTree) {
        BOOST_CHECK_NO_THROW(CondTree());
        auto x = CondTree();
        auto y = x;
        BOOST_CHECK(x.root);
        auto z = std::move(x);
        BOOST_CHECK(!x.root);
    }

    BOOST_AUTO_TEST_CASE(IsTerm_1) {
        const auto root = CondTreeNode();
        BOOST_CHECK_EQUAL(root.isTerm(), true);
    }

    BOOST_AUTO_TEST_CASE(IsTerm_2) {
        auto root = CondTreeNode::make('*');
        root->left = CondTreeNode::make(1);
        root->right = CondTreeNode::make(2);
        BOOST_CHECK_EQUAL(root->isTerm(), false);
    }

    BOOST_AUTO_TEST_CASE(IsValid) {
        auto root = CondTreeNode::make('*');
        BOOST_CHECK(!root->isValid());
        root->left = CondTreeNode::make(1);
        BOOST_CHECK(!root->isValid());
        root->right = CondTreeNode::make(2);
        BOOST_CHECK(root->isValid());
        root->left = CondTreeNode::make('+');
        BOOST_CHECK(!root->isValid());
        root->left->left = CondTreeNode::make(1.2);
        BOOST_CHECK(!root->isValid());
        root->left->right = CondTreeNode::make(2);
        BOOST_CHECK(root->isValid());
        root->right = CondTreeNode::make('-');
        BOOST_CHECK(!root->isValid());
        root->right = CondTreeNode::make(-1.0);
        BOOST_CHECK(root->isValid());
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestValidForm)

    BOOST_AUTO_TEST_CASE(TestValid) {
        const char *buf[] = {
                "1 = y3 = x4",
                "1+1 = y3+z12 = 5*y4",
                "1+1 = 3*y3+3*z12 + 3 = 5 + 2*z3",
        };
        auto sl = SymbolList();
        sl.insert("y");
        sl.insert("z");
        for (const auto s : buf) {
            std::istringstream ss(s);
            BOOST_TEST_CHECKPOINT("parsing " << s);
            for (const auto x : CondParser(ss, sl, "x").parse()) {
                // since the root node is '=', we check the both side
                BOOST_CHECK(isFinalForm(x.root->left));
                BOOST_TEST_PASSPOINT();
                BOOST_CHECK(isFinalForm(x.root->right));
            }
        }
    }

    BOOST_AUTO_TEST_CASE(TestInvalid) {
        const char *buf[] = {
                "1*1 = y3*2 = (1+1)*x4",
                "1+y3*3+z12 = 3*5*z4",
                "1+y3*(3+z12) = 5*z4*4",
                "1/2 = 1-1",
        };
        auto sl = SymbolList();
        sl.insert("y");
        sl.insert("z");
        for (const auto s : buf) {
            std::istringstream ss(s);
            BOOST_TEST_CHECKPOINT("parsing " << s);
            for (const auto x : CondParser(ss, sl, "x").parse()) {
                // since the root node is '=', we check the both side
                BOOST_CHECK(!isFinalForm(x.root->left));
                BOOST_TEST_PASSPOINT();
                BOOST_CHECK(!isFinalForm(x.root->right));
            }
        }
        BOOST_CHECK(!isFinalForm(CondTreeNode::make('+')));
        std::unique_ptr<CondTreeNode> p(new CondTreeNode);
        BOOST_CHECK(!isFinalForm(p));
    }

    BOOST_AUTO_TEST_CASE(TestFinalizable) {
        const char *buf[] = {
                "1-y0 = 0",
                "1 = 1",
                "1+x1 = 1+y2",
                "1+x1 = 1-y2",
                "x1 = (1-y2)+z1",
                "3*(3+(x1*2*(3+6)+5)) - (2+3)*(1-y2)+z1 = 0",
                "1 + x0 = 3",
        };
        auto sl = SymbolList();
        sl.insert("y");
        sl.insert("z");
        for (const auto s : buf) {
            std::istringstream ss(s);
            for (auto x : CondParser(ss, sl, "x").parse()) {
                BOOST_TEST_CHECKPOINT("parsing " << s);
                BOOST_CHECK(!isFinalForm(x.root));
                BOOST_TEST_PASSPOINT();
                BOOST_CHECK(finalizeTree(x) == FinalizationStatus::SUCCESS);
                BOOST_TEST_PASSPOINT();
                BOOST_CHECK(isFinalForm(x.root));
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()
