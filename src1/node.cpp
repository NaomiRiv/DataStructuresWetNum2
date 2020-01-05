#ifndef NODE_
#define NODE_

#include <cmath>
#include <algorithm>

#define NULL_HEIGHT -1

template<typename Key, typename Data>
struct Node {
    Key* key;
    Data* data;

    Node* parent;
    Node* left;
    Node* right;

    int height;
    int bf;

    int GetHeight(Node* node) {
        return (node ? node->height : NULL_HEIGHT);
    }

    void Update() {
        int leftHeight = GetHeight(left);
        int rightHeight = GetHeight(right);

        height = 1 + std::max(leftHeight, rightHeight);
        bf = leftHeight - rightHeight;
    }

    Node<Key, Data>* Successor() {
        Node<Key, Data>* currentNode = this->right;

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

    Node(Key *key = NULL, Data *data = NULL, Node *parent = NULL, Node *left = NULL, Node *right = NULL)
            : key(key), data(data), parent(parent), left(left), right(right), height(0), bf(0) {}

    ~Node() {
        if (key) delete key;
        //if (data) delete data;
    }
};

#endif