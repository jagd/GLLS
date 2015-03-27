#include "condtree.h"

CondTreeNode::CondTreeNode() : type(Type::INVALID_NODE)
{
}

CondTreeNode::CondTreeNode(char op) : type(Type::OP_NODE), op(op)
{
}

CondTreeNode::CondTreeNode(int id) : type(Type::ID_NODE), id(id)
{
}

CondTreeNode::CondTreeNode(double num) : type(Type::NUM_NODE), num(num)
{
}

bool CondTreeNode::isTerm() const
{
    return !(left_ || right_);
}
