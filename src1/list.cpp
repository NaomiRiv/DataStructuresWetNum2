#ifndef LIST_
#define LIST_

#include <iostream>

/* Node in list implementation */
template <typename Key>
struct NodeList {
    Key* key;

    NodeList<Key>* prev;
    NodeList<Key>* next;

    NodeList() : key(nullptr), prev(nullptr), next(nullptr) {}
    NodeList(Key* key) : key(key), prev(nullptr), next(nullptr) {}
    ~NodeList() {
        if (key) {
          delete key;
        }
    }

    Key& operator*() { return *key; }
};
/* -----------------------------------
 * List class implementation
 */
template <typename Key>
class List {
private:
    NodeList<Key>* listStart;
    int listLength;

    void Destroy() {
        NodeList<Key>* currentNode = listStart;
        NodeList<Key>* next = currentNode;
        while (currentNode) {
            next = currentNode->next;
            delete currentNode;
            currentNode = next;
        }
        listLength = 0;
    }
public:
    /* iterator class implementation */
    class iterator {
    private:
        NodeList<Key>* currentNode;
        friend class List<Key>;

    public:
        iterator(NodeList<Key>* node) : currentNode(node) {}
        iterator(iterator const& otherList) = default;

        Key* operator->() { return currentNode->key; }
        Key& operator*() { return *currentNode->key; }
        bool operator==(const iterator& listRef) {
            return (!currentNode && !listRef.currentNode) || listRef.currentNode ? currentNode == listRef.currentNode : false;
        }
        bool operator!=(const iterator& listRef) { return !(*this == listRef); }
        iterator operator++() {
            if (currentNode == nullptr) {
                return *this;
            }
            currentNode = currentNode->next;
            return *this;
        }
        iterator operator++(int) {
            List::iterator it = *this;
            currentNode = currentNode->next;
            return it;
        }
    };
/* -----------------------------------
 * List class methods
 */
     List<Key>& operator=(List<Key> listRef) {
        Destroy();

        listStart = new NodeList<Key>();

        NodeList<Key>* listRefCurr = listRef.listStart->next;

        while (listRefCurr) {
            if (listRefCurr->key) {
                PushBack(*listRefCurr->key);
            }
            listRefCurr = listRefCurr->next;
        }
        return *this;
    }

    List() : listStart(new NodeList<Key>), listLength(0) {}

    List(List<Key> const& otherList) {
        listStart = new NodeList<Key>;

        NodeList<Key>* currentNode = otherList.listStart->next;

        while (currentNode) {
            PushBack(*currentNode->key);
            currentNode = currentNode->next;
        }
    }
    ~List() { Destroy(); }

    int GetLength() const { return listLength; }
    bool IsEmpty() const { return listLength == 0; }

    iterator begin() const { return iterator(listStart->next); }
    iterator end() const { return iterator(nullptr); }

    void PushFront(Key const& keyToPush) {
        NodeList<Key>* nodeToAdd;

        nodeToAdd = new NodeList<Key>(new Key(keyToPush));
        nodeToAdd->next = listStart->next;
        nodeToAdd->prev = listStart;

        if (listStart->next) {
            listStart->next->prev = nodeToAdd;
        }
        listStart->next = nodeToAdd;
        listLength++;
    }

    List<Key>::iterator PushBack(Key const& keyToPush) {
        NodeList<Key>* currentNode = listStart;
        NodeList<Key>* nodeToAdd;
        while (currentNode->next) {
            currentNode = currentNode->next;
        }
        nodeToAdd = new NodeList<Key>(new Key(keyToPush));
        nodeToAdd->prev = currentNode;
        currentNode->next = nodeToAdd;
        listLength++;
        return iterator(nodeToAdd);
    }

    Key* GetFront() {
        if (!listStart->next) {
            return NULL;
        }
        return listStart->next->key;
    }

    void PopFront() {
        if (!listStart->next) {
            return;
        }
        listStart->next->prev = listStart;
        // delete listStart->next;
        listStart->next = listStart->next->next;
        listLength--;
    }

    bool IsKeyInList(const Key keyToFind) const {
        NodeList<Key>* currentNode = listStart->next;
        while (currentNode) {
            if (currentNode->key && *currentNode->key == keyToFind) {
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }

    void DeleteKeyFromList(List<Key>::iterator& iterator) {
        if (iterator == listStart) {
        return;
        }
        NodeList<Key>* currentNode = iterator.currentNode;
        //NodeList<Key>* next = iterator.currentNode->next;
        if (currentNode->prev) {
            currentNode->prev->next = currentNode->next;
        }
        if (currentNode->next) {
            currentNode->next->prev = currentNode->prev;
        }
        delete currentNode;
        listLength--;
        iterator++;
    }

    void RemoveKeyFromList(List<Key>::iterator& iterator) {
        if (iterator == begin()) {
            return;
        }
        NodeList<Key>* currentNode = iterator.currentNode;
        NodeList<Key>* next = iterator.currentNode->next;
        if (currentNode->prev) {
            currentNode->prev->next = currentNode->next;
        }
        if (currentNode->next) {
            currentNode->next->prev = currentNode->prev;
        }
        iterator = iterator(next);
    }


    bool operator==(List<Key>& listRef) {
        List::iterator it = begin();
        List::iterator it2 = listRef.begin();

        while (it != end() || it2 != listRef.end()) {
        if (it == end() || it2 == listRef.end()) {
            return false;
        }
        if (*it != *it2) {
            return false;
        }
        it++;
        it2++;
        }
        return true;
    }

    Key& operator[](int i) {
        if (IsEmpty()) {
            throw std::exception();
        }
        NodeList<Key>* currentNode = listStart->next;
        for (int j = 0; j < i; j++) {
            currentNode = currentNode->next;
        if (!currentNode) {
            throw std::exception();
        }
        }
        return **currentNode;
    }
    bool operator!=(List<Key>& listRef) { return !(*this == listRef); }
};

#endif
