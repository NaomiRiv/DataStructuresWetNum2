#ifndef AVLTree_SR
#define AVLTree_SR

#include <iostream>
#include <functional>
#include <cmath>

#include "serversRankTreeNode.cpp"
#include "src1/list.cpp"
#include "src1/treeExceptions.cpp"

/**
 * how to merge two trees:
 * 1. get the lists from both trees (one for the keys and one for the data): o(n) GetDataList() and  GetDataList()
 * 2. merge both lists by key (four lists - all organized by the corresponding keys list) order: o(n)
 * 3. use FromSortedList function, with the key list and the data list and you'll get an avl tree that contains the wanted keys and wanted data in its nodes
 * */

bool cmp (SRKey lhsSRKey, SRKey rhsSRKey) {
    if (lhsSRKey.traffic < rhsSRKey.traffic) {
        return true;
    } else if (lhsSRKey.traffic > rhsSRKey.traffic) {
        return false;
    } else {
        return lhsSRKey.serversID < rhsSRKey.serversID;
    }
}

class ServersTree{
private:
    SRNode* treeRoot;
    int treeSize;

    /* auxiliary methods */
    static void removeExcessiveNode(ServersTree* avl, SRNode* curr, int wanted); // removes node from the complete tree
    static void insertListIntoAnEmptyAVLTree(SRNode* curr, int length, typename List<SRKey>::iterator& keyListIterator); // inserts list into empty tree
    static SRNode* createACompleteTree(int height); // creates a complete tree
    void ToKeyListAux(List<SRKey>& list, SRNode* currentNode);
    SRNode* GetNode(SRKey const& key, SRNode* currentNode);
    void RemoveKeyFromTreeAux(SRNode* node);
    SRNode* AddToTreeAux(SRNode* nodeToAdd, SRNode* currentNode);
    void RollR(SRNode* node);
    void RollL(SRNode* node);
    void RollRR(SRNode* node);
    void RollLL(SRNode* node);
    void RollRL(SRNode* node);
    void RollLR(SRNode* node);
    void Balance(SRNode* currentNode, bool balanceInsert = true);
    void Destroy(SRNode* currentNode);

//    void printInorderAux(SRNode* node) {
//        if (node == nullptr) {
//            return;
//        }
//        printInorderAux(node->left);
//        std::cout << *(node->data) << " BF: " << node->bf << " Height: " << node->height << std::endl;
//        printInorderAux(node->right);
//    }
public:
    int Rank(SRKey& key, SRNode* currentNode, int r) { // get index of node

        if (currentNode == nullptr) {
            return -1;
        }
        bool lt = cmp(key, *currentNode->key);
        bool gt = cmp(*currentNode->key, key);
        if (!lt && !gt) {
            return r+currentNode->left->r+1; // they are equal
        }
        if (lt) {
            return Rank(key, currentNode->left, r);
        }
        return Rank(key, currentNode->right, r+currentNode->left->r+1);
    }

    SRNode* Select(int k, SRNode* currentNode) { // get node by index
        if (currentNode == nullptr) {
            return nullptr;
        }
        if (currentNode->left->r = k-1) {
            return currentNode;
        } else if (currentNode->left->r > k-1) {
            return Select(k, currentNode->left);
        } else {
            return Select(k-currentNode->left->r-1, currentNode->right);
        }

    }

    int HighestSumAux (int sum, SRNode* currentNode, bool isRightChild) {
        if (!currentNode) {
            return sum;
        }
        if (!isRightChild ) {
            if (currentNode->right) {
                sum += currentNode->right->sum;
            }
            sum += currentNode->key->traffic;
        }
        isRightChild = (currentNode == currentNode->parent->right);
        HighestSumAux(sum, currentNode->parent, isRightChild);
    }

    int HighestSum(int k) {
        int sum = 0;
        int size = GetSize();
        int i = size - k;
        SRNode* startNode = Select(i, treeRoot);
        return HighestSumAux(0, treeRoot, false);
    }
    bool IsEmpty();
    int GetSize();
    bool IsInTree(const SRKey key);
    void AddToTree(SRKey const &keyToAdd);
    void RemoveKeyFromTree(SRKey const &keyToRemove);
    List<SRKey> GetKeyList();
//    void printInorderAVLTree() {
//        std::cout << "---------------------" << std::endl;
//        printInorderAux(treeRoot);
//        std::cout << "---------------------" << std::endl;
//    }
    static ServersTree* FromSortedList(List<SRKey> &keys);
    ServersTree();
    ~ServersTree();
};

/* static and aux methods */


void ServersTree::removeExcessiveNode(ServersTree* avlTree, SRNode* currentNode, int wantedSize) {
    if (avlTree->treeSize == wantedSize) {
        return;
    }
    if (!currentNode) {
        return;
    }
    removeExcessiveNode(avlTree, currentNode->right, wantedSize);
    if (currentNode->IsLeaf()) {
        // delete current node
        if (currentNode->IsRightChild()) {
            currentNode->parent->right = nullptr;
        }
        else {
            currentNode->parent->left = nullptr;
        }
        avlTree->Balance(currentNode->parent);
        // if (currentNode->parent)
        // 	currentNode->parent->update();
        delete currentNode;
        avlTree->treeSize--;
        return;
    }
    removeExcessiveNode(avlTree, currentNode->left, wantedSize);
}


SRNode* ServersTree::createACompleteTree(int height) {
    if (height == 0) {
        return nullptr;
    }
    SRNode *currentNode = new SRNode();

    currentNode->left = createACompleteTree(height - 1);
    currentNode->height = height-1;

    if (currentNode->left) {
        currentNode->left->parent = currentNode;
    }
    currentNode->right = createACompleteTree(height - 1);
    if (currentNode->right) {
        currentNode->right->parent = currentNode;
    }
    return currentNode;
}


void ServersTree::insertListIntoAnEmptyAVLTree(SRNode* currentNode, int length, typename List<SRKey>::iterator& keyListIterator) {
    if (length <= 0) {
        return;
    }
    if (!currentNode) {
        return;
    }
    insertListIntoAnEmptyAVLTree(currentNode->left, length, keyListIterator);

    currentNode->key = new SRKey(*keyListIterator);

    ++keyListIterator;

    insertListIntoAnEmptyAVLTree(currentNode->right, length, keyListIterator);
}


ServersTree* ServersTree::FromSortedList(List<SRKey> &keyList) {
    int numOfNodes = keyList.GetLength();

    int nodesInCompleteTree = 0;
    int height = 0;

    while (nodesInCompleteTree < numOfNodes) {
        nodesInCompleteTree += std::pow(2, height++);
    }
    SRNode* root = ServersTree::createACompleteTree(height); // create a complete empty avl tree
    ServersTree* avlTree = new ServersTree();

    avlTree->treeRoot = root;
    avlTree->treeSize = nodesInCompleteTree;

    removeExcessiveNode(avlTree, root, numOfNodes);

    // we now have a "blank" tree and we need to insert our lists
    typename List<SRKey>::iterator keyListIterator = keyList.begin();

    insertListIntoAnEmptyAVLTree(avlTree->treeRoot, keyList.GetLength(), keyListIterator);

    return avlTree;
}

void updateR(SRNode* curr) {
    if (!curr || !curr->key) return;
    curr->r = (curr->left ? curr->left->r : 0) + (curr->right ? curr->right->r : 0) +1;
}

void updateSum(SRNode* curr) {
    if (!curr || !curr->key) return;
    curr->sum = (curr->left ? curr->left->sum : 0) + (curr->right ? curr->right->sum : 0) + curr->key->traffic;
}

/* AVL tree rolls */

void ServersTree::RollR(SRNode* nodeB) {
	SRNode* nodeA = nodeB->left;
    SRNode* initialParentNode = nodeB->parent;

	if (nodeA->right) {
        nodeA->right->parent = nodeB;
    }
	nodeB->left = nodeA->right;
	nodeA->right = nodeB;
	nodeB->parent = nodeA;
	nodeB->Update();
	nodeA->Update();
	nodeA->parent = initialParentNode;
    updateR(nodeB);
    updateSum(nodeB);
    updateR(nodeA);
    updateSum(nodeA);
	if (initialParentNode == nullptr) {
		treeRoot = nodeA;
        return;
	}
	if (initialParentNode->left == nodeB) {
        initialParentNode->left = nodeA;
    }
	else if (initialParentNode->right == nodeB) {
        initialParentNode->right = nodeA;
    }
}


void ServersTree::RollL(SRNode* nodeA) {
	SRNode* nodeB = nodeA->right;
    SRNode* initialParentNode = nodeA->parent;
    if (nodeB->left) {
        nodeB->left->parent = nodeA;
    }
	nodeA->right = nodeB->left;
	nodeB->left = nodeA;
	nodeA->parent = nodeB;
	nodeA->Update();
	nodeB->Update();
    updateR(nodeB);
    updateSum(nodeB);
    updateR(nodeA);
    updateSum(nodeA);
	nodeB->parent = initialParentNode;
	if (initialParentNode == nullptr) {
		treeRoot = nodeB;
		return;
	}
	if (initialParentNode->left == nodeA) {
        initialParentNode->left = nodeB;
    }
	else if (initialParentNode->right == nodeA) {
        initialParentNode->right = nodeB;
    }
}


void ServersTree::RollRR(SRNode* node) {
	RollL(node);
}


void ServersTree::RollLL(SRNode* node) {
	RollR(node);
}


void ServersTree::RollRL(SRNode* node) {
	RollR(node->right);
	RollL(node);
}


void ServersTree::RollLR(SRNode* node) {
	RollL(node->left);
	RollR(node);
}
// -----------------------------------------------------------------


SRNode* ServersTree::GetNode(SRKey const& key, SRNode* currentNode) {
	if (currentNode == nullptr) {
        return nullptr;
    }
	bool lt = cmp(key, *currentNode->key);
	bool gt = cmp(*currentNode->key, key);
	if (!lt && !gt) {
        return currentNode; // they are equal
    }
	if (lt) {
        return GetNode(key, currentNode->left);
    }
	return GetNode(key, currentNode->right);
}

void ServersTree::RemoveKeyFromTreeAux(SRNode* node) {
	if (node == nullptr) {
        return;
    }
	SRNode* parentNode = node->parent;
    SRNode* replacementNode = nullptr;

	bool isRoot = (treeRoot == node);
	bool isLeftChild = !isRoot && node->IsLeftChild();
	bool isRightChild = !isRoot && node->IsRightChild();

	if (!node->left && !node->right) {
		// node has no children
		if (isLeftChild) {
            parentNode->left = nullptr;
        }
		else if (isRightChild) {
            parentNode->right = nullptr;
        }
		if (node == treeRoot) {
            treeRoot = nullptr;
        }
		Balance(parentNode, false);
		delete node;
	} else {
		if (!node->left || !node->right) {
			// node has one child
			replacementNode = node->left ? node->left : node->right;
			replacementNode->parent = parentNode;
			if (isLeftChild) {
                parentNode->left = replacementNode;
            }
			else if (isRightChild) {
                parentNode->right = replacementNode;
            }
			Balance(parentNode, false);
			delete node;
			if (parentNode == nullptr) {
                treeRoot = replacementNode;
            }
		} else {
			// node has two children
			replacementNode = node->Successor();
			delete node->key;
			node->key = new SRKey(*(replacementNode->key));
			RemoveKeyFromTreeAux(replacementNode);
		}
	}
	updateR(node);
	updateSum(node);
}

SRNode* ServersTree::AddToTreeAux(SRNode* nodeToAdd, SRNode* currentNode) {
	bool lt = cmp(*nodeToAdd->key, *currentNode->key);
	bool gt = cmp(*currentNode->key, *nodeToAdd->key);

	if ((lt && !currentNode->left) || (gt && !currentNode->right)) {
		if (lt) {
            currentNode->left = nodeToAdd;
        }
		else if (gt) {
            currentNode->right = nodeToAdd;
        }
		nodeToAdd->parent = currentNode;
		treeSize++;
		updateR(nodeToAdd);
		updateSum(nodeToAdd);
        updateR(currentNode);
        updateSum(currentNode);
		return nodeToAdd;
	}
	SRNode* ret = AddToTreeAux(nodeToAdd, lt ? currentNode->left : currentNode->right);
    updateR(currentNode);
    updateSum(currentNode);
    return ret;
}


void ServersTree::ToKeyListAux(List<SRKey>& list, SRNode* currentNode) {
	if (currentNode == nullptr) {
        return;
    }
	ToKeyListAux(list, currentNode->left);
	list.PushBack(*currentNode->key);
	ToKeyListAux(list, currentNode->right);
}

void ServersTree::Balance(SRNode* currentNode, bool balanceInsert) {
	if (currentNode == nullptr) {
        return;
    }
	int initialHeight = currentNode->height;
	SRNode* initialCurrentParent = currentNode->parent;
	currentNode->Update();
	if (std::abs(currentNode->bf) > 1) {
		if (currentNode->bf < -1) {
			// rl or rr
			if (currentNode->right->bf <= 0) {
				RollRR(currentNode);
			} else {
				RollRL(currentNode);
			}
		} else if (currentNode->bf > 1) {
			// ll or lr
			if (currentNode->left->bf == -1) {
				RollLR(currentNode);
			} else {
				RollLL(currentNode);
			}
		}
	}
	if (currentNode->height == initialHeight && balanceInsert) {
        return;
    }
	if (initialCurrentParent == nullptr) {
        return;
    }
	Balance(initialCurrentParent);
    updateR(currentNode);
    updateSum(currentNode);
}


void ServersTree::Destroy(SRNode* currentNode) {
	if (currentNode == nullptr) {
	    return;
	}
	SRNode* leftNode = currentNode->left;
    SRNode* rightNode = currentNode->right;
	Destroy(leftNode);
	delete currentNode;
	Destroy(rightNode);
}


ServersTree::ServersTree() : treeRoot(NULL), treeSize(0) {}


bool ServersTree::IsEmpty() {
	return treeSize == 0;
}


int ServersTree::GetSize() {
	return treeSize;
}


bool ServersTree::IsInTree(const SRKey keyToCheck) {
	if (!treeRoot)
		return false;
	return GetNode(keyToCheck, treeRoot) != NULL;
}


void ServersTree::AddToTree(SRKey const &keyToAdd) {
	if (IsInTree(keyToAdd))
		throw Tree::KeyExists(); // throw if AVLTreeIsInTree key
	// allocate new node
	SRKey* newKey;
	SRNode* newNode;
	newKey = new SRKey(keyToAdd);
	newNode = new SRNode(newKey);
	if (!treeRoot) {
		treeRoot = newNode;
		treeSize++;
		updateR(treeRoot);
        updateSum(treeRoot);
        return;
	}
	newNode = AddToTreeAux(newNode, treeRoot);
	if (!newNode->parent)
		return;
	Balance(newNode->parent);
}


void ServersTree::RemoveKeyFromTree(SRKey const& keyToAdd) {
	SRNode* node = GetNode(keyToAdd, treeRoot);
	if (node == nullptr) {
        throw Tree::KeyNotExists();
    }
	RemoveKeyFromTreeAux(node);
	treeSize--;
}


List<SRKey> ServersTree::GetKeyList() {
	List<SRKey> list;
	ToKeyListAux(list, treeRoot);
	return list;
}

ServersTree::~ServersTree() {
	Destroy(treeRoot);
}
#endif
