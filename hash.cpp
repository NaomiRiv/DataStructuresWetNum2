#ifndef _HASH_CPP
#define _HASH_CPP

#include <iostream>
#include "src1/avlTree.cpp"

#define INITIAL 11  // inital hash table size
#define RESIZE 2    // resize factor
#define COLLAPSE 4  // collapse factor

/**
 * Hash table contains the serverID as the keys and as the data contains the dataCentersID and traffic
 * When the data centers merge we won't change the dataCenterID but when
 * the traffic updated we will obviously  update it.
 */

struct HashServersData {
    int dataCenterID;
    int traffic;
};

class Hash {
    // cell in the hash table
    struct Cell {
        AVLTree<int, HashServersData>* chain;

        Cell() : chain(new AVLTree<int, HashServersData>()) {}
        // to destroy the cell we need to destroy the whole chain of the cell
        ~Cell() {
            List<HashServersData*> dataList = chain->GetDataList();

            for (typename List<HashServersData*>::iterator iterator = dataList.begin(); iterator != dataList.end(); ++iterator) {
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
    void insert(int keyToInsert, HashServersData const& dataToInsert, bool checkBalanceSize = true);
    void expand();
    void collapse();

public:
    Hash();

    /* Gets a key and returns if it has been previously inserted into the Hash Table */
    bool IsMember(int x);

    /* Gets a key and data and insert it to the HashTable */
    void Insert(int key, HashServersData const& data);

    /* Gets the data attributed to a key */
    HashServersData Get(int key);

    /* Gets a key and Removes it from the Hash Table */
    HashServersData Remove(int x);

    void Update(int serverID, int traffic) {
        int hashedKey = hash(serverID);

        Cell* cell = tableArray[hashedKey];
        HashServersData* pData;
            pData = cell->chain->GetData(serverID);
            pData->traffic = traffic;
    }

    ~Hash();
};


int Hash::hash(int num) {
    return num % hashSize;
}


void Hash::insert(int key, HashServersData const& data, bool balance) {
    Cell* cellToInsert = tableArray[hash(key)];
    cellToInsert->chain->AddToTree(key, new HashServersData(data));
    hashFilled++;

    if (balance) {
        updateSize();
    }
}


void Hash::updateSize() {
    // change size if needed
    if (hashFilled >= hashSize - 1) {
        expand();
    }
    else if (hashFilled < hashSize / COLLAPSE) {
        collapse();
    }
}


void Hash::expand() {
    int prevSize = hashSize;
    Cell** prevTable = tableArray;

    hashSize *= RESIZE;
    hashFilled = 0;

    tableArray = new Cell*[hashSize];

    for (int i = 0; i < hashSize; i++) {
        tableArray[i] = new Cell();
    }
    for (int i = 0; i < prevSize; i++) {
        List<int> keyList = prevTable[i]->chain->GetKeyList();
        List<HashServersData*> dataList = prevTable[i]->chain->GetDataList();

        typename List<HashServersData*>::iterator dataIterator = dataList.begin();

        for (List<int>::iterator iterator = keyList.begin(); iterator != keyList.end(); ++iterator) {
            insert(*iterator, *(*(dataIterator++)), false);
        }
        delete prevTable[i];
    }
    delete[] prevTable;
}


void Hash::collapse() {
    int prevSize = hashSize;
    Cell** prevTable = tableArray;

    hashSize /= RESIZE;
    hashFilled = 0;

    tableArray = new Cell*[hashSize];

    for (int i = 0; i < hashSize; i++) {
        tableArray[i] = new Cell();
    }
    for (int i = 0; i < prevSize; i++) {
        List<int> keyList = prevTable[i]->chain->GetKeyList();
        List<HashServersData*> dataList = prevTable[i]->chain->GetDataList();

        typename List<HashServersData*>::iterator dataIterator = dataList.begin();

        for (List<int>::iterator iterator = keyList.begin(); iterator != keyList.end(); ++iterator) {
            insert(*iterator, *(*(dataIterator++)), false);
        }
        delete prevTable[i];
    }
    delete[] prevTable;
}


Hash::Hash() : hashSize(INITIAL), hashFilled(0) {
    tableArray = new Cell*[hashSize];

    for (int i = 0; i < hashSize; i++) {
        tableArray[i] = new Cell();
    }
}


void Hash::Insert(int key, HashServersData const& data) {
    insert(key, data);
}


HashServersData Hash::Remove(int key) {
    int hashedKey = hash(key);

    Cell* cell = tableArray[hashedKey];
    HashServersData* pData;

    try {
    pData = cell->chain->GetData(key);
    } catch (Tree::KeyNotExists& e) {
        throw std::range_error("key doesn't exist");
    }
    try {
        cell->chain->RemoveKeyFromTree(key);
    } catch (Tree::KeyNotExists& e) {
        throw std::range_error("key doesn't exist");
    }
    --hashFilled;
    updateSize();

    HashServersData dataToReturn = *pData;

    delete pData;

    return dataToReturn;
}


bool Hash::IsMember(int key) {
    int hashedKey = hash(key);

    return tableArray[hashedKey]->chain->IsInTree(key);
}


HashServersData Hash::Get(int key) {
    Cell* cell = tableArray[hash(key)];
    HashServersData* data;

    try {
        data = cell->chain->GetData(key);
    } catch (Tree::KeyNotExists& e) {
        throw std::range_error("key doesn't exist");
    }
    return *data;
}


Hash::~Hash() {
    for (int i = 0; i < hashSize; i++) {
        delete tableArray[i];
    }
    delete[] tableArray;
}

#endif  // _HASH_CPP
