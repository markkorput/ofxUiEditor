#include "MeshDataManager.h"

using namespace ofxUiEditor;

void MeshDataManager::draw(){
    vector<shared_ptr<MeshData>> rootItems = getRootItems();
    ofSetRectMode(OF_RECTMODE_CORNER);
    
    for(auto &item : rootItems){
        drawItem(item);
    }
}

void MeshDataManager::drawItem(shared_ptr<MeshData> item){
    ofVec3f min = item->getVertBoundsOrigin();
    ofVec3f size = item->getVertBoundsSize();
    
//     ofLog() << "drawItem: " << item->getId()
//        << "\npos: " << item->getPosition()
//        << ", min: " << min
//        << ", size: " << size;

    ofPushMatrix();
    {
        ofTranslate(item->getPosition());
        ofScale(item->getScale());
        ofVec3f rot = item->getRotation();
        ofRotateX(rot.x);
        ofRotateY(rot.y);
        ofRotateZ(rot.z);

        ofPushMatrix();
            auto matrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
            ofTranslate(min);
            // red stroke
            ofSetColor(ofColor::red);
            ofDrawRectangle(0.0f, 0.0f, size.x, size.y);
            // black fill
            ofSetColor(ofColor::black);
            ofDrawRectangle(1.0f/matrix.getScale().x, 1.0/matrix.getScale().y, size.x-2.0f/matrix.getScale().x, size.y-2.0f/matrix.getScale().y);
        ofPopMatrix();
        // children
        for(auto &child : getChildren(item->getId())){
            drawItem(child);
        }
    }
    ofPopMatrix();
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
    newItem->setId(id);

    ofAddListener(newItem->changeEvent, this, &MeshDataManager::onItemChange);
    
    items[id] = newItem;
    ofNotifyEvent(newItemEvent, *newItem);

//    ofLog() << "NEW: " << id;
//    for(auto it=items.begin(); it!=items.end(); it++){
//        ofLog() << " - " << it->first;
//    }

    return newItem;
}

void MeshDataManager::onItemChange(MeshData &item){
    ofNotifyEvent(itemUpdatedEvent, item);
}

vector<shared_ptr<MeshData>> MeshDataManager::getRootItems(){
    vector<shared_ptr<MeshData>> rootItems;
    shared_ptr<MeshData> itemRef = nullptr;

    for(auto it=items.begin(); it!=items.end(); it++){
        // only includes items that have an ID that does not start with the
        // last found root item's id
        if(itemRef == nullptr || it->second->getId().find(itemRef->getId()) != 0)
            rootItems.push_back(itemRef = it->second);
    }

    return rootItems;
}

vector<shared_ptr<MeshData>> MeshDataManager::getChildren(const string &parentId){
    vector<shared_ptr<MeshData>> childItems;
    
    string prefix = parentId;
    if(parentId.find("/") != (parentId.length()-1)){
        prefix += "/";
    }

    // collect all items who's ID starts with the specified parentId
    for(auto it=items.begin(); it!=items.end(); it++){
        // skip items that have an ID that starts with the last found root item's id
        if(it->second->getId().find(prefix) == 0)
            childItems.push_back(it->second);
    }

    return childItems;
}
