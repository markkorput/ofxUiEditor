#include "MeshDataManager.h"

using namespace ofxUiEditor;

void MeshDataManager::draw(){
    
}

shared_ptr<MeshData> MeshDataManager::find(const string &id){
    auto it = items.find(id);
    if(it == items.end())
        return NULL;
    return it->second;
}

shared_ptr<MeshData> MeshDataManager::get(const string &id){
    auto existing = find(id);

    if(existing)
        return existing;

    // create new
    auto newItem = make_shared<MeshData>();
    ofAddListener(newItem->changeEvent, this, &MeshDataManager::onItemChange);

    items[id] = newItem;
    ofNotifyEvent(newItemEvent, *newItem);
    return newItem;
}

void MeshDataManager::onItemChange(MeshData &item){
    ofNotifyEvent(itemUpdatedEvent, item);
}
