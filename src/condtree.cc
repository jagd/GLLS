#include "condtree.h"

#include <memory>
#include <cassert>

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

