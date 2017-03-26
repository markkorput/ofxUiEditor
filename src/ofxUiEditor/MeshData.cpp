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
    updateVertBounds();
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::updateVertBounds(){
    auto x = minmax_element(vertices.begin(), vertices.end(),
                               [](const ofPoint& a, const ofPoint& b) {
                                   return a.x < b.x;
                               });
    auto y = minmax_element(vertices.begin(), vertices.end(),
                               [](const ofPoint& a, const ofPoint& b) {
                                   return a.y < b.y;
                               });
    auto z = minmax_element(vertices.begin(), vertices.end(),
                               [](const ofPoint& a, const ofPoint& b) {
                                   return a.z < b.z;
                               });
    
    vertBoundsOrigin = ofVec3f(x.first->x, y.first->y, z.first->z);
    ofVec3f max = ofVec3f(x.second->x, y.second->y, z.second->z);
    vertBoundsSize = max - vertBoundsOrigin;
}
