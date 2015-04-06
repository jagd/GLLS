#ifndef _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_

#include <memory>
#include <utility>

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
    std::unique_ptr<CondTreeNode> clone() const;
    static std::unique_ptr<CondTreeNode> make(int id);
    static std::unique_ptr<CondTreeNode> make(char op);
    static std::unique_ptr<CondTreeNode> make(double num);
    bool isOp(char op) const { return type == Type::OP_NODE && value.op == op; }
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

/**
    @brief a copyable condition tree
*/
class CondTree
{
public:
    CondTree();
    CondTree(std::unique_ptr<CondTreeNode> r);
    CondTree(const CondTree &ct);
    CondTree(CondTree &&) = default;
    CondTree& operator=(CondTree &&) = default;
    CondTree& operator=(const CondTree &) = default;
    std::unique_ptr<CondTreeNode> root;
};

bool isFinalForm(const std::unique_ptr<CondTreeNode> &);
inline bool isFinalForm(const CondTree &tree)
{
    return isFinalForm(tree.root);
}

enum class FinalizationStatus
{
    SUCCESS,            UNKNOWN_FAILURE,            INVALID_EXPRESSION,
    INVALID_OPERATOR, HIGH_ORDER
};

FinalizationStatus finalizeTree(std::unique_ptr<CondTreeNode> &);
inline FinalizationStatus finalizeTree(CondTree &tree)
{
    return finalizeTree(tree.root);
}

#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDTREE_H_
