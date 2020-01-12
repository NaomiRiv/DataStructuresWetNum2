#ifndef AVLTree_
#define AVLTree_

#include <iostream>
#include <functional>
#include <cmath>

#include "rankNode.cpp"
#include "src1/list.cpp"
#include "src1/treeExceptions.cpp"

/**
 * how to merge two trees:
 * 1. get the lists from both trees (one for the keys and one for the data): o(n) GetDataList() and  GetDataList()
 * 2. merge both lists by key (four lists - all organized by the corresponding keys list) order: o(n)
 * 3. use FromSortedList function, with the key list and the data list and you'll get an avl tree that contains the wanted keys and wanted data in its nodes
 * */

template<typename Key, typename Data = void, typename Compare = std::less<Key> >
class AVLTree{
private:
    RankNode<Key, Data>* treeRoot;
    int treeSize;

    /* auxiliary methods */
    static void removeExcessiveNode(AVLTree<Key, Data, Compare>* avl, RankNode<Key, Data>* curr, int wanted); // removes node from the complete tree
    static void insertListIntoAnEmptyAVLTree(RankNode<Key, Data>* curr, int length, typename List<Key>::iterator& keyListIterator, typename List<Data*>::iterator& dataListIterator); // inserts list into empty tree
    static RankNode<Key, Data>* createACompleteTree(int height); // creates a complete tree
    void ToKeyListAux(List<Key>& list, RankNode<Key, Data>* currentNode);
    void ToDataListAux(List<Data*>& list, RankNode<Key, Data>* currentNode);
    RankNode<Key, Data>* GetNode(Key const& key, RankNode<Key, Data>* currentNode, Compare cmp = Compare());
    void RemoveKeyFromTreeAux(RankNode<Key, Data>* node, Compare cmp = Compare());
    RankNode<Key, Data>* AddToTreeAux(RankNode<Key, Data>* nodeToAdd, RankNode<Key, Data>* currentNode, Compare cmp = Compare());
    void RollR(RankNode<Key, Data>* node, Compare cmp = Compare());
    void RollL(RankNode<Key, Data>* node, Compare cmp = Compare());
    void RollRR(RankNode<Key, Data>* node, Compare cmp = Compare());
    void RollLL(RankNode<Key, Data>* node, Compare cmp = Compare());
    void RollRL(RankNode<Key, Data>* node, Compare cmp = Compare());
    void RollLR(RankNode<Key, Data>* node, Compare cmp = Compare());
    void Balance(RankNode<Key, Data>* currentNode, bool balanceInsert = true);
    void Destroy(RankNode<Key, Data>* currentNode);
    void printInorderAux(RankNode<Key, Data>* node) {
        if (node == nullptr) {
            return;
        }
        printInorderAux(node->left);
        std::cout << *(node->data) << " BF: " << node->bf << " Height: " << node->height << std::endl;
        printInorderAux(node->right);
    }
    RankNode<Key, Data>* Select(int k, RankNode<Key, Data>* node) {
        if (node->left->r = k-1) {
            return node;
        } else if (node->left->r > k-1) {
            return Select(k, node->left);
        } else {
            return Select(k - node->left->r -1, node->right);
        }
    }
public:
    bool IsEmpty();
    int GetSize();
    bool IsInTree(const Key key);
    void AddToTree(Key const &keyToAdd, Data *data = NULL);
    void RemoveKeyFromTree(Key const &keyToRemove);
    List<Key> GetKeyList();
    List<Data*> GetDataList();
    Data* GetData(const Key &key);
    void printInorderAVLTree() {
        std::cout << "---------------------" << std::endl;
        printInorderAux(treeRoot);
        std::cout << "---------------------" << std::endl;
    }
    static AVLTree<Key, Data, Compare>* FromSortedList(List<Key> &keys, List<Data*>& data);
    AVLTree();
    ~AVLTree();
};

/* static and aux methods */

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::removeExcessiveNode(AVLTree<Key, Data, Compare>* avlTree, RankNode<Key, Data>* currentNode, int wantedSize) {
    if (avlTree->treeSize == wantedSize) {
        return;
    }
    if (!currentNode) {
        return;
    }
    removeExcessiveNode(avlTree, currentNode->right, wantedSize);
    if (currentNode->IsLeaf()) {
        // delete current node
        if (currentNode->IsRightSon()) {
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
    removeExcessiveNodes(avlTree, currentNode->left, wantedSize);
}

template<typename Key, typename Data, typename Compare>
RankNode<Key, Data>* AVLTree<Key, Data, Compare>::createACompleteTree(int height) {
    if (height == 0) {
        return nullptr;
    }
    RankNode<Key, Data> *currentNode = new RankNode<Key, Data>();

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

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::insertListIntoAnEmptyAVLTree(RankNode<Key, Data>* currentNode, int length, typename List<Key>::iterator& keyListIterator, typename List<Data*>::iterator& dataListIterator) {
    if (length <= 0) {
        return;
    }
    if (!currentNode) {
        return;
    }
    insertListIntoAnEmptyAVLTree(currentNode->left, length, keyListIterator, dataListIterator);

    currentNode->key = new Key(*keyListIterator);
    currentNode->data = *dataListIterator;

    ++keyListIterator;
    ++dataListIterator;

    insertListIntoAnEmptyAVLTree(currentNode->right, length, keyListIterator, dataListIterator);
}

template<typename Key, typename Data, typename Compare>
AVLTree<Key, Data, Compare>* AVLTree<Key, Data, Compare>::FromSortedList(List<Key> &keyList, List<Data*>& dataList) {
    int numOfNodes = keyList.length();

    int nodesInCompleteTree = 0;
    int height = 0;

    while (nodesInCompleteTree < numOfNodes) {
        nodesInCompleteTree += std::pow(2, height++);
    }
    RankNode<Key, Data>* root = AVLTree<Key, Data, Compare>::createACompleteTree(height); // create a complete empty avl tree
    AVLTree<Key, Data, Compare>* avlTree = new AVLTree<Key, Data, Compare>();

    avlTree->treeRoot = root;
    avlTree->treeSize = nodesInCompleteTree;

    removeExcessiveNodes(avlTree, root, numOfNodes);

    // we now have a "blank" tree and we need to insert our lists
    typename List<Key>::iterator keyListIterator = keyList.begin();
    typename List<Data*>::iterator dataListIterator = dataList.begin();

    insertListIntoAnEmptyAVLTree(avlTree->treeRoot, keyList.length(), keyListIterator, dataListIterator);

    return avlTree;
}

/* AVL tree rolls */
template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RollR(RankNode<Key, Data>* nodeB, Compare cmp) {
	RankNode<Key, Data>* nodeA = nodeB->left;
    RankNode<Key, Data>* initialParentNode = nodeB->parent;
	if (nodeA->right) {
        nodeA->right->parent = nodeB;
    }
	nodeB->left = nodeA->right;
	nodeA->right = nodeB;
	nodeB->parent = nodeA;
	nodeB->Update();
	nodeA->Update();
	nodeA->parent = initialParentNode;
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

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RollL(RankNode<Key, Data>* nodeA, Compare cmp) {
	RankNode<Key, Data>* nodeB = nodeA->right;
    RankNode<Key, Data>* initialParentNode = nodeA->parent;
    if (nodeB->left) {
        nodeB->left->parent = nodeA;
    }
	nodeA->right = nodeB->left;
	nodeB->left = nodeA;
	nodeA->parent = nodeB;
	nodeA->Update();
	nodeB->Update();
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

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RollRR(RankNode<Key, Data>* node, Compare cmp) {
	RollL(node, cmp);
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RollLL(RankNode<Key, Data>* node, Compare cmp) {
	RollR(node, cmp);
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RollRL(RankNode<Key, Data>* node, Compare cmp) {
	RollR(node->right, cmp);
	RollL(node, cmp);
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RollLR(RankNode<Key, Data>* node, Compare cmp) {
	RollL(node->left, cmp);
	RollR(node, cmp);
}
// -----------------------------------------------------------------

template<typename Key, typename Data, typename Compare>
RankNode<Key, Data>* AVLTree<Key, Data, Compare>::GetNode(Key const& key, RankNode<Key, Data>* currentNode, Compare cmp) {
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

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RemoveKeyFromTreeAux(RankNode<Key, Data>* node, Compare cmp) {
	if (node == nullptr) {
        return;
    }
	RankNode<Key, Data>* parentNode = node->parent;
    RankNode<Key, Data>* replacementNode = nullptr;

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
        parentNode->UpdateR();
        parentNode->UpdateSum();
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
            parentNode->UpdateR();
            parentNode->UpdateSum();
			delete node;
			if (parentNode == nullptr) {
                treeRoot = replacementNode;
            }
		} else {
			// node has two children
			replacementNode = node->Successor();
			delete node->key;
			node->key = new Key(*(replacementNode->key));
			node->data = replacementNode->data;
			RemoveKeyFromTreeAux(replacementNode, cmp);
		}
	}
}

template<typename Key, typename Data, typename Compare>
RankNode<Key, Data>* AVLTree<Key, Data, Compare>::AddToTreeAux(RankNode<Key, Data>* nodeToAdd, RankNode<Key, Data>* currentNode, Compare cmp) {
	bool lt = cmp(*nodeToAdd->key, *currentNode->key);
	bool gt = cmp(*currentNode->key, *nodeToAdd->key);
	// found the place to add
	if ((lt && !currentNode->left) || (gt && !currentNode->right)) {
		if (lt) {
            currentNode->left = nodeToAdd;
        }
		else if (gt) {
            currentNode->right = nodeToAdd;
        }
		nodeToAdd->parent = currentNode;
		treeSize++;
		currentNode->UpdateR();
		currentNode->UpdateSum();
		return nodeToAdd;
	}
	// -----------------------
	RankNode<Key, Data>* toReturn = AddToTreeAux(nodeToAdd, lt ? currentNode->left : currentNode->right);
	currentNode->UpdateR();
	currentNode->UpdateSum();
	return toReturn;
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::ToKeyListAux(List<Key>& list, RankNode<Key, Data>* currentNode) {
	if (currentNode == nullptr) {
        return;
    }
	ToKeyListAux(list, currentNode->left);
	list.PushBack(*currentNode->key);
	ToKeyListAux(list, currentNode->right);
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::ToDataListAux(List<Data*>& list, RankNode<Key, Data>* currentNode) {
	if (currentNode == nullptr) {
        return;
    }
	ToDataListAux(list, currentNode->left);
	list.pushBack(currentNode->data);
	ToDataListAux(list, currentNode->right);
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::Balance(RankNode<Key, Data>* currentNode, bool balanceInsert) {
	if (currentNode == nullptr) {
        return;
    }
	int initialHeight = currentNode->height;
	RankNode<Key, Data>* initialCurrentParent = currentNode->parent;
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
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::Destroy(RankNode<Key, Data>* currentNode) {
	if (currentNode == nullptr) {
	    return;
	}
	RankNode<Key, Data>* leftNode = currentNode->left;
    RankNode<Key, Data>* rightNode = currentNode->right;
	Destroy(leftNode);
	delete currentNode;
	Destroy(rightNode);
}

template<typename Key, typename Data, typename Compare>
AVLTree<Key, Data, Compare>::AVLTree() : treeRoot(NULL), treeSize(0) {}

template<typename Key, typename Data, typename Compare>
bool AVLTree<Key, Data, Compare>::IsEmpty() {
	return treeSize == 0;
}

template<typename Key, typename Data, typename Compare>
int AVLTree<Key, Data, Compare>::GetSize() {
	return treeSize;
}

template<typename Key, typename Data, typename Compare>
bool AVLTree<Key, Data, Compare>::IsInTree(const Key keyToCheck) {
	if (!treeRoot)
		return false;
	return GetNode(keyToCheck, treeRoot) != NULL;
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::AddToTree(Key const &keyToAdd, Data *data) {
	if (IsInTree(keyToAdd))
		throw Tree::KeyExists(); // throw if AVLTreeIsInTree key
	// allocate new node
	Key* newKey;
	RankNode<Key, Data>* newNode;
	newKey = new Key(keyToAdd);
	newNode = new RankNode<Key, Data>(newKey, data);
	if (!treeRoot) {
		treeRoot = newNode;
		treeSize++;
		treeRoot->UpdateR();
		treeRoot->UpdateSum();
		return;
	}
	newNode = AddToTreeAux(newNode, treeRoot);
	if (!newNode->parent)
		return;
	Balance(newNode->parent);
}

template<typename Key, typename Data, typename Compare>
void AVLTree<Key, Data, Compare>::RemoveKeyFromTree(Key const& keyToAdd) {
	RankNode<Key, Data>* node = GetNode(keyToAdd, treeRoot);
	if (node == nullptr) {
        throw Tree::KeyNotExists();
    }
	RemoveKeyFromTreeAux(node);
	treeSize--;
}

template<typename Key, typename Data, typename Compare>
List<Key> AVLTree<Key, Data, Compare>::GetKeyList() {
	List<Key> list;
	ToKeyListAux(list, treeRoot);
	return list;
}

template<typename Key, typename Data, typename Compare>
List<Data*> AVLTree<Key, Data, Compare>::GetDataList() {
	List<Data*> list;
	ToDataListAux(list, treeRoot);
	return list;
}

template<typename Key, typename Data, typename Compare>
Data* AVLTree<Key, Data, Compare>::GetData(const Key& key) {
	if (!IsInTree(key)) {
        throw Tree::KeyNotExists();
    }
	RankNode<Key, Data>* nodeForKey = GetNode(key, treeRoot);
	return nodeForKey->data;
}

template<typename Key, typename Data, typename Compare>
AVLTree<Key, Data, Compare>::~AVLTree() {
	Destroy(treeRoot);
}
#endif
