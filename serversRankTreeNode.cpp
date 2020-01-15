#ifndef NODE_SR
#define NODE_SR

#include <cmath>
#include <algorithm>

#define NULL_HEIGHT -1

struct SRKey {
    int traffic;
    int serversID;
};

struct SRNode {
    SRKey* key;

    SRNode* parent;
    SRNode* left;
    SRNode* right;

    int height;
    int bf;

    int r;
    int sum;

    int GetHeight(SRNode* node) {
        return (node ? node->height : NULL_HEIGHT);
    }

    void Update() {
        int leftHeight = GetHeight(left);
        int rightHeight = GetHeight(right);

        height = 1 + std::max(leftHeight, rightHeight);
        bf = leftHeight - rightHeight;
    }

    SRNode* Successor() {
        SRNode* currentNode = this->right;

        while (currentNode->left) {
            currentNode = currentNode->left;
        }
        return currentNode;
    }

    bool IsLeaf() {
        return ((left == nullptr) && (right == nullptr));
    }

    bool IsRightChild() {
        return ((parent != nullptr) && (parent->right == this));
    }

    bool IsLeftChild() {
        return ((parent != nullptr) && (parent->left == this));
    }

    SRNode(SRKey *key = NULL, int r = 0, int sum = 0, SRNode *parent = NULL, SRNode *left = NULL, SRNode *right = NULL)
            : key(key), r(r), sum(sum), parent(parent), left(left), right(right), height(0), bf(0)  {}

    ~SRNode() {
        if (key) delete key;
        //if (data) delete data;
    }
};

#endif