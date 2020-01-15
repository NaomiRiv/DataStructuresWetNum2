#include"library2.h"
#include"DataCenterManager.cpp"
void *Init(int n) {
    return (new DS(n));
}
StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2) {
    if ( DS==NULL ){
        return INVALID_INPUT;
    }
    return static_cast<class DS*>(DS)->MergeDataCenters(dataCenter1, dataCenter2);
}
StatusType AddServer(void *DS, int dataCenterID, int serverID) {
    if ( DS==NULL ){
        return INVALID_INPUT;
    }
    return static_cast<class DS*>(DS)->AddServer(dataCenterID, serverID);
}
StatusType RemoveServer(void *DS, int serverID) {
    if ( DS==NULL ){
        return INVALID_INPUT;
    }
    return static_cast<class DS*>(DS)->RemoveServer(serverID);
}

StatusType SetTraffic(void *DS, int serverID, int traffic){
    if ( DS==NULL ){
        return INVALID_INPUT;
    }
    return static_cast<class DS*>(DS)->SetTraffic(serverID, traffic);
}
StatusType SumHighestTrafficServers(void *DS, int dataCenterID, int k, int *traffic){
    if ( DS==NULL ){
        return INVALID_INPUT;
    }
    return static_cast<class DS*>(DS)->SumHighestTrafficServers(dataCenterID, k, traffic);
}
void Quit(void **DS){
    delete  static_cast<class DS*>(*DS);
    *DS = NULL;
}