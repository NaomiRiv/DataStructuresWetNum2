#ifndef _HASH_CPP
#define _HASH_CPP

#include <iostream>
#include "src1/avlTree.cpp"

#define INITIAL 11  // inital hash table size
#define RESIZE 2    // resize factor
#define COLLAPSE 4  // collapse factor

template <typename Data>
class Hash {
    // cell in the hash table
    struct Cell {
        AVLTree<int, Data>* chain;

        Cell() : chain(new AVLTree<int, Data>()) {}
        // to destroy the cell we need to destroy the whole chain of the cell
        ~Cell() {
            List<Data*> dataList = chain->toDataList();

            for (typename List<Data*>::iterator iterator = dataList.begin(); iterator != dataList.end(); ++iterator) {
                delete *iterator;
            }
            delete chain;
        }
    };

    int hashSize;   // the hash table size
    int hashFilled; // how much is filled

    Cell** tableArray;

    void updateSize();
    int hash(int x);
    void insert(int keyToInsert, Data const& dataToInsert, bool checkBalanceSize = true);
    void expand();
    void collapse();

public:
    Hash();

    /* Gets a key and returns if it has been previously inserted into the Hash Table */
    bool IsMember(int x);

    /* Gets a key and data and insert it to the HashTable */
    void Insert(int key, Data const& data);

    /* Gets the data attributed to a key */
    Data Get(int key);

    /* Gets a key and Removes it from the Hash Table */
    Data Remove(int x);

    ~Hash();
};

template <typename Data>
int Hash<Data>::hash(int num) {
    return num % hashSize;
}

template <typename Data>
void Hash<Data>::insert(int key, Data const& data, bool balance) {
    Cell* cellToInsert = tableArray[hash(key)];
    cellToInsert->chain->insert(key, new Data(data));
    hashFilled++;

    if (balance) {
        updateSize();
    }
}


template <typename Data>
void Hash<Data>::updateSize() {
    // change size if needed
    if (hashFilled >= hashSize - 1) {
        expand();
    }
    else if (hashFilled < hashSize / COLLAPSE) {
        collapse();
    }
}

template <typename Data>
void Hash<Data>::expand() {
    int prevSize = hashSize;
    Cell** prevTable = tableArray;

    hashSize *= RESIZE;
    hashFilled = 0;

    tableArray = new Cell*[hashSize];

    for (int i = 0; i < hashSize; i++) {
        tableArray[i] = new Cell();
    }
    for (int i = 0; i < prevSize; i++) {
        List<int> keyList = prevTable[i]->chain->toKeyList();
        List<Data*> dataList = prevTable[i]->chain->toDataList();

        typename List<Data*>::iterator dataIterator = dataList.begin();

        for (List<int>::iterator iterator = keyList.begin(); iterator != keyList.end(); ++iterator) {
            insert(*iterator, *(*(dataIterator++)), false);
        }
        delete prevTable[i];
    }
    delete[] prevTable;
}

template <typename Data>
void Hash<Data>::collapse() {
    int prevSize = hashSize;
    Cell** prevTable = tableArray;

    hashSize /= RESIZE;
    hashFilled = 0;

    tableArray = new Cell*[hashSize];

    for (int i = 0; i < hashSize; i++) {
        tableArray[i] = new Cell();
    }
    for (int i = 0; i < prevSize; i++) {
        List<int> keyList = prevTable[i]->chain->toKeyList();
        List<Data*> dataList = prevTable[i]->chain->toDataList();

        typename List<Data*>::iterator dataIterator = dataList.begin();

        for (List<int>::iterator iterator = keyList.begin(); iterator != keyList.end(); ++iterator) {
            insert(*iterator, *(*(dataIterator++)), false);
        }
        delete prevTable[i];
    }
    delete[] prevTable;
}

template <typename Data>
Hash<Data>::Hash() : hashSize(INITIAL), hashFilled(0) {
    tableArray = new Cell*[hashSize];

    for (int i = 0; i < hashSize; i++) {
        tableArray[i] = new Cell();
    }
}

template <typename Data>
void Hash<Data>::Insert(int key, Data const& data) {
    insert(key, data);
}

template <typename Data>
Data Hash<Data>::Remove(int key) {
    int hashedKey = hash(key);

    Cell* cell = tableArray[hashedKey];
    Data* pData;

    try {
    pData = cell->chain->getData(key);
    } catch (Tree::KeyNotExists& e) {
        throw std::range_error("key doesn't exist");
    }
    try {
        cell->chain->erase(key);
    } catch (Tree::KeyNotExists& e) {
        throw std::range_error("key doesn't exist");
    }
    --hashFilled;
    updateSize();

    Data dataToReturn = *pData;

    delete pData;

    return dataToReturn;
}

template <typename Data>
bool Hash<Data>::IsMember(int key) {
    int hashedKey = hash(key);

    return tableArray[hashedKey]->chain->contains(key);
}

template <typename Data>
Data Hash<Data>::Get(int key) {
    Cell* cell = tableArray[hash(key)];
    Data* data;

    try {
        data = cell->chain->getData(key);
    } catch (Tree::KeyNotExists& e) {
        throw std::range_error("key doesn't exist");
    }
    return *data;
}

template <typename Data>
Hash<Data>::~Hash() {
    for (int i = 0; i < hashSize; i++) {
        delete tableArray[i];
    }
    delete[] tableArray;
}

#endif  // _HASH_CPP
