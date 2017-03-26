#include "MeshData.h"

using namespace ofxUiEditor;

void MeshData::setPosition(const ofVec3f &pos){
    position = pos;
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::setRotation(const ofVec3f &rot){
    rotation = rot;
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::setScale(const ofVec3f &scale){
    this->scale = scale;
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::setVertex(int idx, const ofVec3f &vert){
    // grow list if necessary
    for(int i=vertices.size(); i<(idx+1); i++){
        vertices.push_back(ofVec3f());
    }

    vertices[idx] = vert;
    ofNotifyEvent(changeEvent, *this);
}
