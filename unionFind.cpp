#ifndef _UNION_FIND_CPP
#define _UNION_FIND_CPP

#include <stdexcept>
#include <iostream>
#include "serversRankTree.cpp"

#define TO_INDEX(i) (i-1)
#define TO_SET(i) (i+1)

void ValidateI(int i, int len) {
  if (i <= 0 || i > len) {
    throw std::range_error("out of range");
  }
}

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

    ServersTree** dataArray;
    UFNode** nodeArray;
public:
    /* Initializes UnionFind with an array of Data and its length */
    UnionFind(int len);

    /* Gets two indexes, unites the set and returns the index corresponding to the new set */
    int Union(int i, int j);

    /* Return the index which corresponds to the Set which the index is associated with */
    int Find(int i);

    /* Finds the data which corresponds with the index and returns it */
    ServersTree GetData(int index);

    void UpdateData(int dataCenterID, int serverID, int newTraffic, int oldTraffic = -1) {
        UFNode* set = nodeArray[TO_INDEX(Find(dataCenterID))];
        ServersTree* tree = dataArray[TO_INDEX(set->i)];
        if (oldTraffic != -1) {
            tree->RemoveKeyFromTree({oldTraffic, serverID});
            tree->AddToTree({newTraffic, serverID});
        } else {
            tree->AddToTree({newTraffic, serverID});
        }
    }

    void RemoveData(int dataCenterID, int serverID, int traffic) {
        UFNode* set = nodeArray[TO_INDEX(Find(dataCenterID))];
        ServersTree* tree = dataArray[TO_INDEX(set->i)];
        if (tree->IsInTree({traffic, serverID})) {
            tree->RemoveKeyFromTree({traffic, serverID});
        }
    }

    static List<SRKey>* merge(ServersTree* lhsTree, ServersTree* rhsTree) {
        List<SRKey> lhsKeys = lhsTree->GetKeyList();
        List<SRKey> rhsKeys = lhsTree->GetKeyList();

        List<SRKey>* ret = new List<SRKey>();

        typename List<SRKey>::iterator lhsIt = lhsKeys.begin();
        typename List<SRKey>::iterator rhsIt = rhsKeys.begin();

        while(lhsIt != lhsKeys.end() && rhsIt != rhsKeys.end()) {
            if (cmp(*lhsIt, *rhsIt)) {
                ret->PushBack(*lhsIt);
                lhsIt++;
            } else {
                ret->PushBack(*rhsIt);
                rhsIt++;
            }
        }
        while (lhsIt != lhsKeys.end()) {
            ret->PushBack(*lhsIt);
            lhsIt++;
        }
        while (rhsIt != rhsKeys.end()) {
            ret->PushBack(*rhsIt);
            rhsIt++;
        }
        return ret;
    }

    /* Destroys the UnionFind */
    ~UnionFind();
};


UnionFind::UnionFind(int n) {
    if (n <= 0) {
      throw std::range_error("length is negative");
    }
    length = n;

    dataArray = new ServersTree*[n];
    nodeArray = new UFNode*[n];

    for (int i = 0; i < n; i++) {
    dataArray[i] = new ServersTree();
    nodeArray[i] = new UFNode(TO_SET(i));
    }
}


int UnionFind::Find(int i) {
    ValidateI(i, length);

    UFNode* node = nodeArray[TO_INDEX(i)];
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


ServersTree UnionFind::GetData(int i) {
    ValidateI(i, length);

    return *dataArray[Find(i)];
}

int UnionFind::Union(int i, int j) {
    ValidateI(i, length);
    ValidateI(j, length);

    //int index1 = i-1;
    //int index2 = i-1;

    UFNode* set1 = nodeArray[TO_INDEX(Find(i))];
    UFNode* set2 = nodeArray[TO_INDEX(Find(j))];

    if (i == j || set1 == set2) {
        return i;
    }
    //if (nodeArray[TO_INDEX(i)] != set1 || nodeArray[TO_INDEX(j)] != set2) {
    //    throw std::invalid_argument("Not sets");
    //}
    UFNode* bigSet = set1;
    UFNode* smallSet = set2;

    if (set1->size < set2->size) {
        std::swap(bigSet, smallSet);
    }
    smallSet->parent = bigSet;
    bigSet->size += smallSet->size;

    List<SRKey>* merged = merge(dataArray[bigSet->i], dataArray[smallSet->i]); // merge servers

    ServersTree* newTree = ServersTree::FromSortedList(*merged);

    delete merged;

    ServersTree* oldTree = dataArray[TO_INDEX(bigSet->i)];
    dataArray[TO_INDEX(bigSet->i)] = newTree;
    delete oldTree;

    return bigSet->i;
}


UnionFind::~UnionFind() {
    for (int i = 0; i < length; i++) {
        delete nodeArray[i];
        delete dataArray[i];
    }
    delete[] nodeArray;
    delete[] dataArray;
}
#endif  //_UNION_FIND_CPP
