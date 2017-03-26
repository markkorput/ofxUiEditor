//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"

// ofxGui addon
#include "ofXml.h"
// ofxOsc addon
#include "ofxOscReceiver.h"
// ofxUiEditor addon
#include "ofxUiEditor.h"

using namespace ofxUiEditor;

const string paramsFile = "params.xml";
const string layoutFile = "layout.json";

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void exit(ofEventArgs &args);

private: // attributes
    ofxOscReceiver oscReceiver;
    MeshDataManager meshDataManager;

    ofEasyCam cam;
};

//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------


void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example");
    ofSetWindowShape(400,300);

    // load layout
    ofLog() << "Loading layouts from " << layoutFile;
    meshDataManager.loadFromFile(layoutFile);
    
    // setup osc message listener
    oscReceiver.setup(8080);
}

void ofApp::update(){
    const int MAX_MESSAGES = 20;

    for(int i=0; i<MAX_MESSAGES; i++){
        if(!oscReceiver.hasWaitingMessages())
            break;

        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);

        meshDataManager.processOscMessage(msg);
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

void ofApp::exit(ofEventArgs &args){
    ofLog() << "Saving layouts to " << layoutFile;
    meshDataManager.saveToFile(layoutFile);
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
