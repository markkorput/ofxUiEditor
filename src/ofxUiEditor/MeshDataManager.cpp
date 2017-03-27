#include "MeshDataManager.h"
#include "ofxJSON.h"

//
// Local helper methods
//

ofVec3f getVec3f(const Json::Value & jsonValue){
    if(jsonValue.isNull())
        return ofVec3f(0.0f);

    vector<string> floats = ofSplitString(jsonValue.asString(), ", ");

    if(floats.size() != 3)
        return ofVec3f(0.0f);

    return ofVec3f(ofToFloat(floats[0]), ofToFloat(floats[1]), ofToFloat(floats[2]));
}

//
// MeshDataManager implementation
//

using namespace ofxUiEditor;

void MeshDataManager::draw(){
    vector<shared_ptr<MeshData>> rootItems = getRootItems();
    // ofSetRectMode(OF_RECTMODE_CORNER);

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

bool MeshDataManager::saveToFile(const string& filePath){
    ofxJSONElement json, meshesEl;

    for(auto it=items.begin(); it!=items.end(); it++){
        auto data = it->second;

        ofxJSONElement el;
        el["position"] = ofToString(data->getPosition());
        el["rotation"] = ofToString(data->getRotation());
        el["scale"] = ofToString(data->getScale());

        Json::Value verticesJson(Json::arrayValue);
        auto& verts = data->getVertices();
        for(int i=0; i<verts.size(); i++)
            verticesJson[i] = ofToString(verts[i]);
        el["vertices"] = verticesJson;

        meshesEl[it->first] = el;
    }

    json["meshes"] = meshesEl;
    return json.save(filePath, true);
}

bool MeshDataManager::loadFromFile(const string& filePath){
    ofxJSONElement json;

    if(!json.open(filePath)){
        ofLogWarning() << "Could not load file: " << filePath;
        return false;
    }

    for(auto it=json.begin(); it!=json.end(); it++){
        if(it.key().asString() == "meshes"){
            auto meshesJson = json[it.key().asString()];
            for(auto it2=meshesJson.begin(); it2!=meshesJson.end(); it2++){
                string id = it2.key().asString();
                auto itemJson = meshesJson[id];

                auto item = get(id);
                item->setPosition(getVec3f(itemJson["position"]));
                item->setRotation(getVec3f(itemJson["rotation"]));
                item->setScale(getVec3f(itemJson["scale"]));

                auto vertsJson = itemJson["vertices"];
                if(!vertsJson.isArray())
                    continue;

                vector<ofVec3f> verts;
                for(int i=0; i<vertsJson.size(); i++){
                    // item->setVertex(i, getVec3f(vertsJson[i]));
                    verts.push_back(getVec3f(vertsJson[i]));
                }
                
                item->setVertices(verts);
            }
            continue;
        }


        ofLogWarning() << "Unknown JSON key: " << it.key().asString();
    }

    return true;
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
        // only includes items that have an ID that does not
        // start with the last found root item's id
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

#ifdef OFX_UI_EDITOR_OSC
bool MeshDataManager::processOscMessage(ofxOscMessage msg){
    // ofLog() << msg.getAddress();

    if(msg.getAddress() == "/ui-editor/mesh/position"){
        // args; [s, f,f,f] // Mesh-id and Vec3f
        get(msg.getArgAsString(0))->setPosition(ofVec3f(msg.getArgAsFloat(1),
                                                        msg.getArgAsFloat(2),
                                                        msg.getArgAsFloat(3)));
        // yes, we processed the message
        return true;
    }

    if(msg.getAddress() == "/ui-editor/mesh/rotation"){
        // args; [s, f,f,f] // Mesh-id and Vec3f
        get(msg.getArgAsString(0))->setRotation(ofVec3f(msg.getArgAsFloat(1),
                                                        msg.getArgAsFloat(2),
                                                        msg.getArgAsFloat(3)));
        // yes, we processed the message
        return true;
    }

    if(msg.getAddress() == "/ui-editor/mesh/scale"){
        // args; [s, f,f,f] // Mesh-id and Vec3f
        get(msg.getArgAsString(0))->setScale(ofVec3f(msg.getArgAsFloat(1),
                                                     msg.getArgAsFloat(2),
                                                     msg.getArgAsFloat(3)));
        // yes, we processed the message
        return true;
    }

    if(msg.getAddress() == "/ui-editor/mesh/vertices"){
        // args; [s ,f,f,f ,f,f,f ,...] // Mesh-id and X-times Vec3f
        auto itemRef = get(msg.getArgAsString(0));

        for(int idx=0; msg.getNumArgs() >= 1+(idx+1)*3; idx++){
            itemRef->setVertex(idx, ofVec3f(msg.getArgAsFloat(1+idx*3),
                                             msg.getArgAsFloat(1+idx*3+1),
                                             msg.getArgAsFloat(1+idx*3+2)));
        }

        // yes, we processed the message
        return true;
    }

    return false;
}

#endif // #ifdef OFX_UI_EDITOR_OSC
