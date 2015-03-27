#ifndef _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_

#include <memory>

class CondTreeNode
{
public:
    enum class Type {INVALID_NODE, ID_NODE, OP_NODE, NUM_NODE};
    CondTreeNode();
    CondTreeNode(CondTreeNode &&) = default;
    ~CondTreeNode();
    explicit CondTreeNode(int id);
    explicit CondTreeNode(char op);
    explicit CondTreeNode(double num);
    static std::unique_ptr<CondTreeNode> make(int id);
    static std::unique_ptr<CondTreeNode> make(char op);
    static std::unique_ptr<CondTreeNode> make(double num);
    void removeLeaves();
    bool isTerm() const;
    bool isValid() const;
    Type type;
    union {
        char op;
        int id;
        double num;
    };
    std::unique_ptr<CondTreeNode> left;
    std::unique_ptr<CondTreeNode> right;
private:
    CondTreeNode(const CondTreeNode &) = delete;
};


#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
