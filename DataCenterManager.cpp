#include "library2.h"
#include "src1/avlTree.cpp"
#include "unionFind.cpp"
#include "hash.cpp"
#include <assert.h>


class DS {
private:
    int DataCentersCount;
    int ServersCount;
    ServersTree *mainTree;
    Hash *mainHash;
    UnionFind *mainUnion;

public:

    DS(int n)  {
        DataCentersCount = n;
        ServersCount = 0;
        mainTree = new ServersTree();
        mainHash = new Hash();
        mainUnion = new UnionFind(n);
    }

    ~DS() {
        if (mainTree) delete mainTree;
        if (mainHash) delete mainHash;
        if (mainUnion) delete mainUnion;
    }

    StatusType MergeDataCenters(int dataCenter1, int dataCenter2) {
        if (dataCenter1 > DataCentersCount ||  dataCenter1<=0 || dataCenter2 > DataCentersCount || dataCenter2<=0){
            return INVALID_INPUT;
        }
        mainUnion->Union(dataCenter1,dataCenter2);
        return SUCCESS;
    }

    StatusType AddServer(int dataCenterID, int serverID){
        if(dataCenterID > DataCentersCount || dataCenterID<=0 || serverID<=0 ){
            return INVALID_INPUT;
        }
        if(mainHash->IsMember(serverID)){
            return FAILURE;
        }
        HashServersData ServerToAdd;
        ServerToAdd.dataCenterID = dataCenterID;
        ServerToAdd.traffic = 0;
        mainHash->Insert(serverID, ServerToAdd);
        ServersCount++;
        return SUCCESS;
    }

    StatusType RemoveServer(int serverID){
        if(serverID<=0){
            return INVALID_INPUT;
        }
        if(!mainHash->IsMember(serverID)){
            return FAILURE;
        }
        HashServersData ServerToDelete = mainHash->Get(serverID);
        int hisDataCenter = ServerToDelete.dataCenterID;
        int hisTraffic = ServerToDelete.traffic;
        mainHash->Remove(serverID);
        SRKey keyToDelete;
        keyToDelete.serversID = serverID;
        keyToDelete.traffic = hisTraffic;
        int num1 = mainUnion->Find(hisDataCenter);
        mainUnion->GetData(num1).RemoveKeyFromTree(keyToDelete);
        mainTree->RemoveKeyFromTree(keyToDelete);
        ServersCount--;
        return SUCCESS;
    }

    StatusType SetTraffic(int serverID, int traffic){
        if(serverID<=0 || traffic<0){
            return INVALID_INPUT;
        }
        if(!mainHash->IsMember(serverID)){
            return FAILURE;
        }
        HashServersData ServerToSet = mainHash->Get(serverID);
        int hisDataCenter = ServerToSet.dataCenterID;
        int hisFormerTraffic = ServerToSet.traffic; //save his old traffic to check if he exists in trees
        ServerToSet.traffic = traffic; //update traffic in hashtable
        SRKey keyToCheck;
        keyToCheck.serversID = serverID;
        keyToCheck.traffic = hisFormerTraffic;
        SRKey keyToSet;
        keyToSet.serversID = serverID;
        keyToSet.traffic = traffic;
        int num1 = mainUnion->Find(hisDataCenter);
        if(!mainTree->IsInTree(keyToCheck)){
            mainTree->AddToTree(keyToSet);
            mainUnion->GetData(num1).AddToTree(keyToSet);
            return SUCCESS;
        }
        mainTree->RemoveKeyFromTree(keyToCheck);
        mainTree->AddToTree(keyToSet);
        mainUnion->GetData(num1).RemoveKeyFromTree(keyToCheck);
        mainUnion->GetData(num1).AddToTree(keyToSet);
        return SUCCESS;
    }

    //**if k is bigger then number of servers just return all of them
    StatusType SumHighestTrafficServers(int dataCenterID, int k, int *traffic){
        if(dataCenterID<0 || traffic==NULL || k<0 || dataCenterID>DataCentersCount){
            return INVALID_INPUT;
        }
        if(dataCenterID==0){
            *traffic = mainTree->HighestSum( k);
            return SUCCESS;
        }
            int num1 = mainUnion->Find(dataCenterID);
            *traffic = mainUnion->GetData(num1).HighestSum( k);
            return SUCCESS;
    }

};