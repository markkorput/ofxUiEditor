//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"

// ofxGui addon
#include "ofXml.h"
// ofxOsc addon
#include "ofxOscReceiver.h"
// ofxInterface
#include "ofxInterface.h"
// ofxUiEditor addon
#include "ofxUiEditor.h"

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
    ofxUiEditor::MeshDataManager meshDataManager;

    shared_ptr<ofxInterface::Node> sceneRef;

    ofEasyCam cam;
    bool bDrawDebug, bDrawManager;
};

//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------


void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example");
    ofSetWindowShape(400,300);

    // load layouts
    ofLog() << "Loading layouts from " << layoutFile;
//    meshDataManager.loadFromFile(layoutFile);
    
    // create scene
    sceneRef = make_shared<ofxInterface::Node>();
    sceneRef->setSize(ofGetWidth(), ofGetHeight());
    sceneRef->setName("ofxUiEditor-example-scene");

    // populate scene with layout
    auto layoutNode = meshDataManager.generateNode<ofxInterface::Node>("frame.panel");
    if(layoutNode){
        sceneRef->addChild(layoutNode);
        layoutNode->setPosition(1, 1);
    }

    // setup osc message listener
    oscReceiver.setup(8080);
    bDrawManager = false;
    bDrawDebug = true;
    
    ofSetRectMode(OF_RECTMODE_CENTER);
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
    ofScale(100.0f, 100.0f, 100.0f);

    if(bDrawManager)
        meshDataManager.draw();

    sceneRef->render();
    
    if(bDrawDebug)
        sceneRef->renderDebug();

    cam.end();
}

void ofApp::keyPressed(int key){
    if(key == 'd'){
        bDrawDebug = !bDrawDebug;
    }
    
    if(key == 'm'){
        bDrawManager = !bDrawManager;
    }
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
