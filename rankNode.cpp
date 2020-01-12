#ifndef NODE_
#define NODE_

#include <cmath>
#include <algorithm>

#define NULL_HEIGHT -1

template<typename Key, typename Data>
struct RankNode {
    Key* key;
    Data* data;

    RankNode* parent;
    RankNode* left;
    RankNode* right;

    int height;
    int bf;

    int GetHeight(RankNode* node) {
        return (node ? node->height : NULL_HEIGHT);
    }

    void Update() {
        int leftHeight = GetHeight(left);
        int rightHeight = GetHeight(right);

        height = 1 + std::max(leftHeight, rightHeight);
        bf = leftHeight - rightHeight;
    }

    RankNode<Key, Data>* Successor() {
        RankNode<Key, Data>* currentRankNode = this->right;

        while (currentRankNode->left) {
            currentRankNode = currentRankNode->left;
        }
        return currentRankNode;
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

    RankNode(Key *key = NULL, Data *data = NULL, RankNode *parent = NULL, RankNode *left = NULL, RankNode *right = NULL)
            : key(key), data(data), parent(parent), left(left), right(right), height(0), bf(0) {}

    ~RankNode() {
        if (key) delete key;
        //if (data) delete data;
    }
};

#endif