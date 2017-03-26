//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"

// ofxOsc
#include "ofxOscReceiver.h"

// ofxUiEditor
#include "ofxUiEditor.h"

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

    ofEasyCam cam;
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

        // ofLog() << msg.getAddress();

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
    ofBackground(ofColor::gray);
    
    cam.begin();
    ofScale(50.0f, 50.0f, 50.0f);
    ofDrawRectangle(0.0f, 0.0f, 1.0f, 1.0f);
    meshDataManager.draw();
    cam.end();
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
