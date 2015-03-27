#ifndef _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_

#include <memory>

class CondTreeNode
{
public:
    enum class Type {INVALID_NODE, ID_NODE, OP_NODE, NUM_NODE};
    CondTreeNode();
    CondTreeNode(const CondTreeNode &);
    CondTreeNode(CondTreeNode &&) = default;
    explicit CondTreeNode(int id);
    explicit CondTreeNode(char op);
    explicit CondTreeNode(double num);
    static std::unique_ptr<CondTreeNode> make(int id);
    static std::unique_ptr<CondTreeNode> make(char op);
    static std::unique_ptr<CondTreeNode> make(double num);
    bool isTerm() const;
    bool isValid() const;
    Type type;
    /** non-anonymous union is more convenient for the copy ctor */
    union {
        char op;
        int id;
        double num;
    } value;
    std::unique_ptr<CondTreeNode> left;
    std::unique_ptr<CondTreeNode> right;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
