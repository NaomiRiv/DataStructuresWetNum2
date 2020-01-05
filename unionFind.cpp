#ifndef _UNION_FIND_CPP
#define _UNION_FIND_CPP

#include <stdexcept>
#include <iostream>

void ValidateI(int i, int len) {
  if (i < 0 || i >= len) {
    throw std::range_error("out of range");
  }
}

template <typename Data>
class UnionFind {
    class UFNode {
        friend class UnionFind;
    public:
        int i;
        int size;

        UFNode* parent;
        UFNode(int i, UFNode* parent = nullptr, int size = 1) : i(i), parent(parent), size(size) {}
    };
    int length;

    Data** dataArray;
    UFNode** nodeArray;
public:
    /* Initializes UnionFind with an array of Data and its length */
    UnionFind(Data* dataArray, int len);

    /* Gets two indexes, unites the set and returns the index corresponding to the new set */
    int Unite(int i, int j);

    /* Return the index which corresponds to the Set which the index is associated with */
    int Find(int i);

    /* Finds the data which corresponds with the index and returns it */
    Data GetData(int index);

    /* Destroys the UnionFind */
    ~UnionFind();
};

template <typename Data>
UnionFind<Data>::UnionFind(Data* dataArray, int len) {
    if (len < 0) {
      throw std::range_error("length is negative");
    }
    length = len;

    dataArray = new Data*[len];
    nodeArray = new UFNode*[len];

    for (int i = 0; i < len; i++) {
    dataArray[i] = new Data(dataArray[i]);
    nodeArray[i] = new UFNode(i);
    }
}

template <typename Data>
int UnionFind<Data>::Find(int i) {
    ValidateI(i, length);

    UFNode* node = nodeArray[i];
    UFNode* currentNode = node;

    while (node->parent) {
        node = node->parent;
    }
    while (currentNode->parent) {
        UFNode* oldParent = currentNode->parent;

        currentNode->parent = node;
        currentNode = oldParent;
    }
    return node->i;
}

template <typename Data>
Data UnionFind<Data>::GetData(int i) {
    ValidateI(i, length);

    return *dataArray[Find(i)];
}

template <typename Data>
int UnionFind<Data>::Unite(int i, int j) {
    ValidateI(i, length);
    ValidateI(j, length);

    UFNode* set1 = nodeArray[Find(i)];
    UFNode* set2 = nodeArray[Find(j)];

    if (i == j || set1 == set2) {
        return i;
    }
    if (nodeArray[i] != set1 || nodeArray[j] != set2) {
        throw std::invalid_argument("Not sets");
    }
    UFNode* bigSet = set1;
    UFNode* smallSet = set2;

    if (set1->size < set2->size) {
        std::swap(set1, set2);
    }
    smallSet->parent = bigSet;
    bigSet->size += smallSet->size;

    return bigSet->i;
}

template <typename Data>
UnionFind<Data>::~UnionFind() {
    for (int i = 0; i < length; i++) {
        delete nodeArray[i];
        delete dataArray[i];
    }
    delete[] nodeArray;
    delete[] dataArray;
}
#endif  //_UNION_FIND_CPP
