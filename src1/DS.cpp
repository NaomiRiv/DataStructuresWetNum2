//
// Created by nomi on 12/12/19.
//
#include "library1.h"
#include "avlTree.cpp"
#include "dataCenter.cpp"
#include <assert.h>

class OSInDataCenterCount {
public:
    int osTypeCount;
    int dataCenterID;

    friend bool operator<(const OSInDataCenterCount& lhs, const OSInDataCenterCount& rhs) {
        if (lhs.osTypeCount > rhs.osTypeCount) return true;
        else if (lhs.osTypeCount < rhs.osTypeCount) return false;
        else return (lhs.dataCenterID < rhs.dataCenterID);
    }
    friend bool operator==(const OSInDataCenterCount& lhs, const OSInDataCenterCount& rhs) {
        return ((lhs.osTypeCount == rhs.osTypeCount) && (lhs.dataCenterID == rhs.dataCenterID));
    }

    explicit OSInDataCenterCount(int dataCenterID, int osTypeCount = 0) : osTypeCount(osTypeCount), dataCenterID(dataCenterID) {}
};


class DS {
private:
    int dataCentersCount;

    AVLTree<int, DataCenter, std::less<int>>* pDataCenters;
    AVLTree<OSInDataCenterCount, int>* pLinuxDataCenters;
    AVLTree<OSInDataCenterCount, int>* pWindowsDataCenters;

public:

    DS() : dataCentersCount(0) {
        pDataCenters = new AVLTree<int, DataCenter, std::less<int>>();
        pLinuxDataCenters = new AVLTree<OSInDataCenterCount, int>();
        pWindowsDataCenters = new AVLTree<OSInDataCenterCount, int>();
    }

    ~DS() {
        if (pDataCenters) delete pDataCenters;
        if (pLinuxDataCenters) delete pLinuxDataCenters;
        if (pWindowsDataCenters) delete pWindowsDataCenters;
    }
    StatusType AddDataCenter(int dataCenterID, int numOfServers) {
        if (numOfServers <= 0 || dataCenterID <= 0) {
            return INVALID_INPUT;
        }
        if (pDataCenters->IsInTree(dataCenterID)) {
            return FAILURE;
        }
        DataCenter *pDataCenterToAdd = new DataCenter(numOfServers, dataCenterID);
        try {
            pDataCenters->AddToTree(dataCenterID, pDataCenterToAdd);
            dataCentersCount++;;
            OSInDataCenterCount dataCenterLinuxCount(dataCenterID, numOfServers);
            OSInDataCenterCount dataCenterWindowsCount(dataCenterID);

            pLinuxDataCenters->AddToTree(dataCenterLinuxCount);
            pWindowsDataCenters->AddToTree(dataCenterWindowsCount);
        } catch (const std::exception &) {
            delete pDataCenterToAdd;
            return FAILURE;
        }
        return SUCCESS;
    }
    StatusType RemoveDataCenter(int dataCenterID) {
        if (dataCenterID <= 0) {
            return INVALID_INPUT;
        }
        if (!pDataCenters->IsInTree(dataCenterID)) {
            return FAILURE;
        }
        try {
            DataCenter *pDataCenter = pDataCenters->GetData(dataCenterID);
            if (!pDataCenter) return FAILURE;
            OSInDataCenterCount dataCenterLinuxCount(dataCenterID, pDataCenter->numOfLinuxServers);
            OSInDataCenterCount dataCenterWindowsCount(dataCenterID, pDataCenter->numOfWindowsServers);

            pLinuxDataCenters->RemoveKeyFromTree(dataCenterLinuxCount);
            pWindowsDataCenters->RemoveKeyFromTree(dataCenterWindowsCount);

            pDataCenters->RemoveKeyFromTree(dataCenterID);
            dataCentersCount--;
        } catch (const std::exception &) {
            return FAILURE;
        }
        return SUCCESS;
    }

    void UpdateLists(DataCenter* pDataCenter, int dataCenterID, int linuxCount, int windowsCount, enum OS oldOS, enum OS newOS) {

        OSInDataCenterCount dataCenterLinuxCount(dataCenterID, linuxCount);
        OSInDataCenterCount dataCenterWindowsCount(dataCenterID, windowsCount);

        if (oldOS != newOS) {
            pLinuxDataCenters->RemoveKeyFromTree(dataCenterLinuxCount);
            (oldOS == linuxOS) ? dataCenterLinuxCount.osTypeCount-- : dataCenterLinuxCount.osTypeCount++;
            pLinuxDataCenters->AddToTree(dataCenterLinuxCount);
            pWindowsDataCenters->RemoveKeyFromTree(dataCenterWindowsCount);
            (oldOS == linuxOS) ? dataCenterWindowsCount.osTypeCount++ : dataCenterWindowsCount.osTypeCount--;
            pWindowsDataCenters->AddToTree(dataCenterWindowsCount);
        }
    }

    void UpdateServerData(DataCenter* pDataCenter, Server* pServer, enum OS os) {
        // update os lists
        pServer->MoveServerFromListToList((os == linuxOS) ? &pDataCenter->occupiedLinuxServers
                                                          : &pDataCenter->occupiedWindowsServers);

        // update selected server
        pServer->UpdateIsAvailable(false);
        pServer->UpdateOS((enum OS) os);

        //Update List ptr in server[i] and the iterator
        pServer->UpdateServerIDList((os == linuxOS) ? &pDataCenter->occupiedLinuxServers
                                                    : &pDataCenter->occupiedWindowsServers);
    }

    StatusType RequestServer(int dataCenterID, int serverID, int os, int *assignedID) {
        if (dataCenterID <= 0 || serverID < 0 || os < 0 || os > 1 || assignedID == NULL) {
            return INVALID_INPUT;
        }
        try {
            DataCenter *pDataCenter = pDataCenters->GetData(dataCenterID);
            if (!pDataCenter) return FAILURE;
            if (serverID >= pDataCenter->GetNumOfServers()) return INVALID_INPUT;
            Server *pServer = pDataCenter->GetServerByServerID(serverID);
            if (!pServer) return FAILURE;
            enum OS oldOS;
            // server is the wanted server, we will check if he is available first
            if (pServer->IsAvailable()) {
                // the wanted server is available we now will take it and update its OS

                // first we need to get the old OS to update trees if needed
                oldOS = pServer->GetOS();

                UpdateServerData(pDataCenter,pServer, (enum OS) os);
                    // --------------------
                // update the return value
                *assignedID = serverID;
            } else {
                // the wanted server is occupied, we will find the next server that can be used by the order
                *assignedID = pDataCenter->GetServerIDAndList((enum OS) os, &oldOS);
                if (*assignedID == -1) return FAILURE;

                // we have the serverID we want to assign
                pServer = pDataCenter->GetServerByServerID(*assignedID);

                UpdateServerData(pDataCenter, pServer, (enum OS) os);
            }

            // update os trees
            UpdateLists(pDataCenter, dataCenterID, pDataCenter->numOfLinuxServers, pDataCenter->numOfWindowsServers, oldOS, (enum OS) os);
            pDataCenter->UpdateOSCount(oldOS, (enum OS) os);

        } catch (std::exception &) {
            return FAILURE;
        }
        return SUCCESS;
    }

    StatusType FreeServer(int dataCenterID, int serverID) {
        if (dataCenterID <= 0 || serverID < 0) {
            return INVALID_INPUT;
        }
        try {
            DataCenter *pDataCenter = pDataCenters->GetData(dataCenterID);

            if (!pDataCenter) return FAILURE;
            if (pDataCenter->GetNumOfServers() <= serverID) return INVALID_INPUT;
            Server *pServer = pDataCenter->GetServerByServerID(serverID);
            if (!pServer) return FAILURE;
            if (pServer->IsAvailable()) return FAILURE;
            pServer->UpdateIsAvailable(true);
            if (pServer->GetOS() == linuxOS) {
                pServer->MoveServerFromListToList(&pDataCenter->availableLinuxServers);
            } else {
                pServer->MoveServerFromListToList(&pDataCenter->availableWindowsServers);
            }
        } catch(const std::exception &) {

            return FAILURE;
        }
        return SUCCESS;
    }
    StatusType GetDataCentersByOS(int os, int **dataCenters, int* numOfDataCenters) {
        if (dataCenters == NULL || numOfDataCenters == NULL || os < 0 || os > 1) {
            return INVALID_INPUT;
        }
        try {
            List<OSInDataCenterCount> listOSInDatCenterCount;
            *dataCenters = (int *) malloc(dataCentersCount * sizeof **dataCenters);
            *numOfDataCenters = dataCentersCount;
            listOSInDatCenterCount = (os == linuxOS) ? pLinuxDataCenters->GetKeyList()
                                                     : pWindowsDataCenters->GetKeyList();;
            List<OSInDataCenterCount>::iterator it = listOSInDatCenterCount.begin();
            if (!dataCentersCount) return FAILURE;
            int i = 0;
            while (it != listOSInDatCenterCount.end()) {
                (*dataCenters)[i] = it->dataCenterID;
                it++;
                i++;
            }
        } catch (std::exception &) {
            return FAILURE;
        }
        return SUCCESS;
    }
};