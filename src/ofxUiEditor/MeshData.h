#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class MeshData : public ofParameterGroup {
        
    public:
        const string CHILD_SEPARATOR = "/";
        const string TYPE_SEPARATOR = ".";

    public:
        MeshData() : position(ofVec3f(0.0f)),
                        rotation(ofVec3f(0.0f)),
                        scale(ofVec3f(1.0f)){}

    public: // getters/setters
        const string& getId() const { return id; }
        void setId(const string &newId){ id = newId; }
        
        string getLocalId() const;
        string getName() const;
        string getType() const;

        const ofVec3f& getPosition() const { return position; }
        void setPosition(const ofVec3f &pos);

        const ofVec3f& getRotation() const { return rotation; }
        void setRotation(const ofVec3f &rot);

        const ofVec3f& getScale() const { return scale; }
        void setScale(const ofVec3f &scale);

        void setVertex(int idx, const ofVec3f &vert, bool update=true, bool notify=true);
        void setVertices(const vector<ofVec3f>& verts);

        const ofVec3f& getVertBoundsOrigin() const { return vertBoundsOrigin; }
        const ofVec3f& getVertBoundsSize() const { return vertBoundsSize; }

        const vector<ofVec3f>& getVertices() const { return vertices; }

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
