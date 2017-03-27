#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class MeshData : public ofParameterGroup {

    public:
        MeshData() : position(ofVec3f(0.0f)),
                        rotation(ofVec3f(0.0f)),
                        scale(ofVec3f(1.0f)){}

    public: // getters/setters
        const string& getId(){ return id; }
        void setId(const string &newId){ id = newId; }

        const ofVec3f& getPosition(){ return position; }
        void setPosition(const ofVec3f &pos);

        const ofVec3f& getRotation(){ return rotation; }
        void setRotation(const ofVec3f &rot);

        const ofVec3f& getScale(){ return scale; }
        void setScale(const ofVec3f &scale);

        void setVertex(int idx, const ofVec3f &vert);
        const ofVec3f& getVertBoundsOrigin(){ return vertBoundsOrigin; }
        const ofVec3f& getVertBoundsSize(){ return vertBoundsSize; }

        const vector<ofVec3f>& getVertices(){ return vertices; }

        ofVec3f getOrigin();

    private: // helpers methods
        
        void updateVertBounds();
        
    public: // events
        ofEvent<MeshData> changeEvent;

    private: // attributes
        string id;
        ofVec3f position, rotation, scale;
        vector<ofVec3f> vertices;

        ofVec3f vertBoundsOrigin, vertBoundsSize;
    };
};
