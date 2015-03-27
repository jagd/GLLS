#include "condtree.h"

#include <memory>

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
