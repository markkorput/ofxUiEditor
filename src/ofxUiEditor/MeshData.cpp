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
    updateVertBounds();
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::setVertex(int idx, const ofVec3f &vert, bool update, bool notify){
    // grow list if necessary
    for(int i=vertices.size(); i<(idx+1); i++){
        vertices.push_back(ofVec3f());
    }

    // overwrite specified vertex
    vertices[idx] = vert;

    // recalculate vertex-dependent bounds attributes
    updateVertBounds();
    // notify
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::setVertices(const vector<ofVec3f>& verts){
    vertices.clear();
    vertices = verts;
    // recalculate vertex-dependent bounds attributes
    updateVertBounds();
    // notify
    ofNotifyEvent(changeEvent, *this);
}

void MeshData::updateVertBounds(){
    if(vertices.empty()){
        vertBoundsOrigin = vertBoundsSize = ofVec3f(0.0f);
        return;
    }

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
    vertBoundsSize = ofVec3f(x.second->x, y.second->y, z.second->z) - vertBoundsOrigin;
}

ofVec3f MeshData::getOrigin(){
    ofVec3f boundsOrigin = vertBoundsOrigin;
    boundsOrigin.rotate(rotation.x, rotation.y, rotation.z);
    return boundsOrigin+position;
}
