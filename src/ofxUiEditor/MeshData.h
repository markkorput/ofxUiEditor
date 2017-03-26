#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class MeshData {

    public: // getters/setters
        void setPosition(const ofVec3f &pos);
        void setRotation(const ofVec3f &rot);
        void setScale(const ofVec3f &scale);
        void setVertex(int idx, const ofVec3f &vert);

    public: // events
        ofEvent<MeshData> changeEvent;

    private: // attributes
        string id;
        ofVec3f position, rotation, scale;
        vector<ofVec3f> vertices;
    };
};
