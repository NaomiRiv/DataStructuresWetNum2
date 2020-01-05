//
// Created by nomi on 12/12/19.
//
#include "library1.h"
#include "DS.cpp"

void* Init() {
    return (new DS());
}
StatusType AddDataCenter(void *DS, int dataCenterID, int numOfServers) {
    if (!DS) return INVALID_INPUT;
    return static_cast<class DS*>(DS)->AddDataCenter(dataCenterID, numOfServers);
}
StatusType RemoveDataCenter(void *DS, int dataCenterID) {
    if (!DS) return INVALID_INPUT;
    return static_cast<class DS*>(DS)->RemoveDataCenter(dataCenterID);
}
StatusType RequestServer(void *DS, int dataCenterID, int serverID, int os, int *assignedID) {
    if (!DS) return INVALID_INPUT;
    return static_cast<class DS*>(DS)->RequestServer(dataCenterID, serverID, os, assignedID);
}
StatusType FreeServer(void *DS, int dataCenterID, int serverID) {
    if (!DS) return INVALID_INPUT;
    return static_cast<class DS*>(DS)->FreeServer(dataCenterID, serverID);
}
StatusType GetDataCentersByOS(void *DS, int os, int **dataCenters, int* numOfDataCenters) {
    if (!DS) return INVALID_INPUT;
    return static_cast<class DS*>(DS)->GetDataCentersByOS(os, dataCenters, numOfDataCenters);
}
void Quit(void **DS) {
    delete  static_cast<class DS*>(*DS);
    *DS = NULL;
}