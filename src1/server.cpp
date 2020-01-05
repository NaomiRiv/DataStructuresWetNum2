#include "osTypes.cpp"
#include "list.cpp"
class Server {
private:
    bool isAvailable;
    enum OS OS;
    List<int>::iterator serverIDIterator;
    List<int>* pServerIDList;
public:
    explicit Server(List<int>::iterator serverIDIterator, List<int>* pServerIDList, bool isAvailable = true, enum OS OS = linuxOS) : isAvailable(isAvailable), OS(OS), serverIDIterator(serverIDIterator), pServerIDList(pServerIDList) {}

    bool IsAvailable() { return isAvailable; }
    enum OS GetOS() { return OS; }

    void UpdateIsAvailable(bool status) { isAvailable = status; }
    void UpdateOS(enum OS newOS) { OS = newOS; }
    void UpdateServerIDList(List<int>* list) { pServerIDList = list; }
    List<int>::iterator* GetServerIDIterator() { return &serverIDIterator; }
    void MoveServerFromListToList(List<int>* pToList) {
        int serverID = *serverIDIterator;
        pServerIDList->DeleteKeyFromList(serverIDIterator);
        serverIDIterator = pToList->PushBack(serverID);
        pServerIDList = pToList;
    }
    List<int>* GetServerIDList() { return pServerIDList; }

    ~Server() {
    }
};
