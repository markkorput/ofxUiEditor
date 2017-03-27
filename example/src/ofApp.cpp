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

    bool loadLayouts(const string& createSceneNode="");
    bool saveLayouts();

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

    // create scene
    sceneRef = make_shared<ofxInterface::Node>();
    sceneRef->setSize(ofGetWidth(), ofGetHeight());
    sceneRef->setName("ofxUiEditor-example-scene");

    loadLayouts("panel.frame");

    // setup osc message listener
    oscReceiver.setup(8080);
    bDrawManager = false;
    bDrawDebug = true;
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
        return;
    }
    
    if(key == 'm'){
        bDrawManager = !bDrawManager;
        return;
    }
    
    if(key=='l'){
        loadLayouts("panel.frame");
        return;
    }

    if(key=='s'){
        saveLayouts();
        return;
    }
}

void ofApp::exit(ofEventArgs &args){
//    saveLayouts();
}

bool ofApp::loadLayouts(const string& createSceneNode){
    // clear scene
    while(sceneRef->getNumChildren() > 0)
        sceneRef->removeChild(0);

    ofLog() << "Loading layouts from " << layoutFile;
    if(!meshDataManager.loadFromFile(layoutFile))
        return false;

    if(createSceneNode != ""){
        auto layoutNode = meshDataManager.generateNode<ofxInterface::Node>(createSceneNode);
        if(layoutNode){
            layoutNode->setPosition(0,0,0);
            sceneRef->addChild(layoutNode);
        }
    }
    return true;
}

bool ofApp::saveLayouts(){
    ofLog() << "Saving layouts to " << layoutFile;
    return meshDataManager.saveToFile(layoutFile);
}


//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
