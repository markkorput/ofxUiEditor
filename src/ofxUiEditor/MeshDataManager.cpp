#include "MeshDataManager.h"

using namespace ofxUiEditor;

MeshData* MeshDataManager::find(const string &id){
    auto it = items.find(id);
    if(it == items.end())
        return NULL;
    return &it->second;
}

MeshData* MeshDataManager::get(const string &id){
    auto existing = find(id);

    if(existing)
        return existing;

    // create new
    MeshData *pNew = &items[id];
    ofNotifyEvent(newItemEvent, *pNew);
    ofAddListener(pNew->changeEvent, this, &MeshDataManager::onItemChange);
    return pNew;
}

void MeshDataManager::onItemChange(MeshData &item){
    ofNotifyEvent(itemUpdatedEvent, item);
}
