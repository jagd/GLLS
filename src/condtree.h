#ifndef _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_

#include <memory>

class CondTreeNode
{
public:
    enum class Type {INVALID_NODE, ID_NODE, OP_NODE, NUM_NODE};
    CondTreeNode();
    explicit CondTreeNode(int id);
    explicit CondTreeNode(char op);
    explicit CondTreeNode(double num);
    bool isTerm() const;
    Type type;
    union {
        char op;
        int id;
        double num;
    };
private:
    std::unique_ptr<CondTreeNode> left_;
    std::unique_ptr<CondTreeNode> right_;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
