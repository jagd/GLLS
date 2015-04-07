#include "condtree.h"
#include <memory>
#include <utility>
#include <cassert>

static FinalizationStatus finalizePlus(std::unique_ptr<CondTreeNode> &root);
static FinalizationStatus finalizeMinus(std::unique_ptr<CondTreeNode> &root);
static FinalizationStatus finalizeMultiply(std::unique_ptr<CondTreeNode> &root);
static FinalizationStatus finalizeDivide(std::unique_ptr<CondTreeNode> &root);
static FinalizationStatus finalizeTreeImpl(std::unique_ptr<CondTreeNode> &root);
static bool isFinalFormImpl(const std::unique_ptr<CondTreeNode> &root);

CondTreeNode::CondTreeNode() : type(Type::INVALID_NODE)
{
}

CondTreeNode::CondTreeNode(char op) : type(Type::OP_NODE)
{
    value.op = op;
}

CondTreeNode::CondTreeNode(int id) : type(Type::ID_NODE)
{
    value.id = id;
}

CondTreeNode::CondTreeNode(double num) : type(Type::NUM_NODE)
{
    value.num = num;
}

bool CondTreeNode::isTerm() const
{
    return !(left || right);
}

std::unique_ptr<CondTreeNode> CondTreeNode::make(int id)
{
    /* there is no make_unique() in C++11, but make_shared */
    return std::unique_ptr<CondTreeNode>(new CondTreeNode(id));
}

std::unique_ptr<CondTreeNode> CondTreeNode::make(char op)
{
    return std::unique_ptr<CondTreeNode>(new CondTreeNode(op));
}

std::unique_ptr<CondTreeNode> CondTreeNode::make(double num)
{
    return std::unique_ptr<CondTreeNode>(new CondTreeNode(num));
}

bool CondTreeNode::isValid() const
{
    return
        (
              !(left || right)
            && (type == Type::NUM_NODE || type == Type::ID_NODE)
        ) || (
               (type == Type::OP_NODE)
            && left && right
            && left->isValid() && right->isValid()
        );
}

CondTreeNode::CondTreeNode(const CondTreeNode &root)
{
    type = root.type;
    value = root.value;
    if (root.left) {
        left.reset(new CondTreeNode(*(root.left)));
    }
    if (root.right) {
        right.reset(new CondTreeNode(*(root.right)));
    }
}

std::unique_ptr<CondTreeNode> CondTreeNode::clone() const
{
    return std::unique_ptr<CondTreeNode>(new CondTreeNode(*this));
}

CondTree::CondTree() : root(new CondTreeNode())
{
}

CondTree::CondTree(const CondTree &ct)
{
    assert(ct.root);
    root = ct.root->clone();
}

CondTree::CondTree(std::unique_ptr<CondTreeNode> r) : root(std::move(r))
{
}

static bool isFinalFormImpl(const std::unique_ptr<CondTreeNode> &root)
{
    switch (root->type) {
        case CondTreeNode::Type::ID_NODE:
            return root->isTerm();
        case CondTreeNode::Type::NUM_NODE:
            return root->isTerm();
        case CondTreeNode::Type::OP_NODE:
            switch (root->value.op) {
                case '+':
                    return isFinalForm(root->left) && isFinalForm(root->right);
                case '*':
                    return root->left->type == CondTreeNode::Type::NUM_NODE
                        && root->right->type == CondTreeNode::Type::ID_NODE;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return false;
}

bool isFinalForm(const std::unique_ptr<CondTreeNode> &root)
{
    if (!(root && root->isValid())) {
        return false;
    }
    return isFinalFormImpl(root);
}

FinalizationStatus finalizePlus(std::unique_ptr<CondTreeNode> &root)
{
    assert(root->isOp('+'));
    const auto N = CondTreeNode::Type::NUM_NODE;
    if (root->left->type == N && root->right->type == N) {
        root = CondTreeNode::make(root->left->value.num+root->left->value.num);
    }
    return FinalizationStatus::SUCCESS;
}

static FinalizationStatus finalizeMinus(std::unique_ptr<CondTreeNode> &root)
{
    assert(root->isOp('-'));
    const auto N = CondTreeNode::Type::NUM_NODE;
    if (root->left->type == N && root->right->type == N) {
        root = CondTreeNode::make(root->left->value.num-root->left->value.num);
        return FinalizationStatus::SUCCESS;
    } else {
        // don't change anything
        auto n = CondTreeNode::make('+');
        n->right = CondTreeNode::make('*');
        n->right->left = CondTreeNode::make(-1.0);
        // now exception safe
        n->right->right = std::move(root->right);
        n->left = std::move(root->left);
        root = std::move(n);
        return finalizeMultiply(root->right);
    }
}

/** @return true if the invoker should pass */
static bool auxFinalizeMultiplyNum(std::unique_ptr<CondTreeNode> &root)
{
    assert(root->isOp('*'));
    const auto N = CondTreeNode::Type::NUM_NODE;
    if (root->right->type == N) {
        if (root->left->type == N) {
            root = CondTreeNode::make(
                    root->left->value.num * root->left->value.num
            );
            return true;
        } else {
            std::swap(root->left, root->right);
        }
    }
    return false;
}

static bool auxFinalizeMultiplyPlus(
        std::unique_ptr<CondTreeNode> &root,
        FinalizationStatus &s
)
{
    assert(root->isOp('*'));
    bool pass = false;
    while (root->right->isOp('+')) {
        pass = true;
        // ensure exception safe
        auto n = CondTreeNode::make('+');
        n->left = CondTreeNode::make('*');
        n->left->left = root->left->clone();
        n->left->right = root->right->left->clone();
        n->right = CondTreeNode::make('*');
        // following: nothrow
        n->right->left = std::move(root->left);
        n->right->right = std::move(root->right->right);
        root = std::move(n);
        const auto nls = finalizeMultiply(root->left);
        if (nls != FinalizationStatus::SUCCESS) {
            s =  nls;
            return true;
        }
        const auto nrs = finalizeMultiply(root->right);
        if (nrs != FinalizationStatus::SUCCESS) {
            s =  nrs;
            return true;
        }
        if (root->left->isOp('+')) {
            std::swap(root->left, root->right);
        }
    }
    return pass;
}

static bool auxFinalizeMultiplyMul(
        std::unique_ptr<CondTreeNode> &root,
        FinalizationStatus &s
)
{
    assert(root->isOp('*'));
    if (root->right->isOp('*')) {
        std::swap(root->left, root->right->right);
        std::swap(root->left, root->right);
        s = finalizeMultiply(root->left);
        if (s != FinalizationStatus::SUCCESS) {
            return true;
        }
        s = finalizeMultiply(root);
        if (s != FinalizationStatus::SUCCESS) {
            return true;
        }
    }
    return false;
}

static FinalizationStatus finalizeMultiply(std::unique_ptr<CondTreeNode> &root) {
    if (auxFinalizeMultiplyNum(root)) {
        return FinalizationStatus::SUCCESS;
    }
    FinalizationStatus res;
    if (auxFinalizeMultiplyMul(root,res)) {
        return res;
    }
    if (auxFinalizeMultiplyPlus(root,res)) {
        return res;
    }
    assert(root->left->type == CondTreeNode::Type::NUM_NODE);
    switch (root->left->type) {
        case CondTreeNode::Type::NUM_NODE:
            return FinalizationStatus::SUCCESS;
        case CondTreeNode::Type::ID_NODE:
            return FinalizationStatus::HIGH_ORDER;
        default:
            return FinalizationStatus::UNKNOWN_FAILURE;
    }
}

static FinalizationStatus finalizeDivide(std::unique_ptr<CondTreeNode> &root)
{
    assert(root->isOp('/'));
}

static FinalizationStatus finalizeTreeImpl(std::unique_ptr<CondTreeNode> &root)
{
    if (root->isTerm()) {
        return FinalizationStatus::SUCCESS;
    }
    const auto sl = finalizeTreeImpl(root->left);
    if (sl !=  FinalizationStatus::SUCCESS) {
        return sl;
    }
    const auto sr = finalizeTreeImpl(root->right);
    if (sr !=  FinalizationStatus::SUCCESS) {
        return sr;
    }
    assert(root->type == CondTreeNode::Type::OP_NODE);
    switch (root->value.op) {
        case '+':
            return finalizePlus(root);
        case '-':
            return finalizeMinus(root);
        case '*':
            return finalizeMultiply(root);
        case '/':
            return finalizeDivide(root);
        default:
            return FinalizationStatus::INVALID_OPERATOR;
    }
//    return FinalizationStatus::UNKNOWN_FAILURE;
}

FinalizationStatus finalizeTree(std::unique_ptr<CondTreeNode> &root)
{
    if (!root->isValid()) {
        return FinalizationStatus::INVALID_EXPRESSION;
    }
    return finalizeTreeImpl(root);
}
