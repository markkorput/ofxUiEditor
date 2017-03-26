//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"

// ofxOsc
#include "ofxOscReceiver.h"


namespace ofxUiEditor {
    class MeshData {
        
    public: // getters/setters
        void setPosition(const ofVec3f &pos){
            position = pos;
            ofNotifyEvent(changeEvent, *this);
        }

        void setRotation(const ofVec3f &rot){
            rotation = rot;
            ofNotifyEvent(changeEvent, *this);
        }

        void setScale(const ofVec3f &scale){
            this->scale = scale;
            ofNotifyEvent(changeEvent, *this);
        }

        void setVertex(int idx, const ofVec3f &vert){
            // grow list if necessary
            for(int i=vertices.size(); i<(idx+1); i++){
                vertices.push_back(ofVec3f());
            }
            
            vertices[idx] = vert;
            ofNotifyEvent(changeEvent, *this);
        }

    public: // events
        ofEvent<MeshData> changeEvent;

    private: // attributes
        string id;
        ofVec3f position, rotation, scale;
        vector<ofVec3f> vertices;
    };
    
    class MeshDataManager {
        
    public: // getters/setters
        MeshData* find(const string &id){
            auto it = items.find(id);
            if(it == items.end())
                return NULL;
            return &it->second;
        }

        MeshData* get(const string &id){
            auto existing = find(id);

            if(existing)
                return existing;

            // create new
            MeshData *pNew = &items[id];
            ofNotifyEvent(newItemEvent, *pNew);
            ofAddListener(pNew->changeEvent, this, &MeshDataManager::onItemChange);
            return pNew;
        }
        
        void onItemChange(MeshData &item){
            ofNotifyEvent(itemUpdatedEvent, item);
        }

    public: // events
        ofEvent<MeshData> newItemEvent, itemUpdatedEvent;

    private: // attributes
        map<string, MeshData> items;
    };
}

using namespace ofxUiEditor;

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void keyPressed(int key);

    private: // attributes
        ofxOscReceiver oscReceiver;
        MeshDataManager meshDataManager;
};

//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------


void ofApp::setup(){
    ofSetWindowTitle("ofxUiEditor - example");
    ofSetWindowShape(400,300);
    
    oscReceiver.setup(8080);
}

void ofApp::update(){
    const int MAX_MESSAGES = 20;

    for(int i=0; i<MAX_MESSAGES; i++){
        if(!oscReceiver.hasWaitingMessages())
            break;

        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        
        if(msg.getAddress() == "/ui-editor/mesh/position"){
            // args; [s, f,f,f] // Mesh-id and Vec3f
            meshDataManager.get(msg.getArgAsString(0))->setPosition(
                ofVec3f(msg.getArgAsFloat(1),
                        msg.getArgAsFloat(2),
                        msg.getArgAsFloat(3)));
            continue;
        }

        if(msg.getAddress() == "/ui-editor/mesh/rotation"){
            // args; [s, f,f,f] // Mesh-id and Vec3f
            meshDataManager.get(msg.getArgAsString(0))->setRotation(
                ofVec3f(msg.getArgAsFloat(1),
                        msg.getArgAsFloat(2),
                        msg.getArgAsFloat(3)));
            continue;
        }

        if(msg.getAddress() == "/ui-editor/mesh/scale"){
            // args; [s, f,f,f] // Mesh-id and Vec3f
            meshDataManager.get(msg.getArgAsString(0))->setScale(
                ofVec3f(msg.getArgAsFloat(1),
                        msg.getArgAsFloat(2),
                        msg.getArgAsFloat(3)));
            continue;
        }
        
        if(msg.getAddress() == "/ui-editor/mesh/vertices"){
            // args; [s ,f,f,f ,f,f,f ,...] // Mesh-id and X-times Vec3f
            auto pItem = meshDataManager.get(msg.getArgAsString(0));
            for(int idx=0; msg.getNumArgs() >= 1+(idx+1)*3; idx++){
                pItem->setVertex(idx,
                                 ofVec3f(msg.getArgAsFloat(1+idx*3),
                                         msg.getArgAsFloat(1+idx*3+1),
                                         msg.getArgAsFloat(1+idx*3+2)));
            }
            continue;
        }
    }
}

void ofApp::draw(){
    ofBackground(ofColor::lightGreen);
}

void ofApp::keyPressed(int key){
}


//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
