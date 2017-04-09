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
// local
#include "components/AbortButton.h"
#include "components/SubmitButton.h"


const string paramsFile = "params.xml";
const string layoutFile = "layout.json";

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void exit(ofEventArgs &args);

private:

    bool loadLayouts(const string& createSceneNode="");
    bool saveLayouts();

    void onGeneratedNodeUpdated(ofxInterface::Node& node);

private: // attributes
    ofxOscReceiver oscReceiver;
    ofxUiEditor::MeshDataManager meshDataManager;
    ofxUiEditor::NodeGenerator<ofxInterface::Node> nodeGenerator;

    shared_ptr<ofxInterface::Node> sceneRef;
    ofxInterface::Node* layoutNode;

    ofEasyCam cam;
    bool bDrawDebug, bDrawManager, bCamEnabled;
};

//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example");
    ofSetWindowShape(800,600);

    // attributes
    bDrawManager = false;
    bDrawDebug = bCamEnabled = true;

    nodeGenerator.setup(meshDataManager);
    ofAddListener(nodeGenerator.nodeUpdatedEvent, this, &ofApp::onGeneratedNodeUpdated);

    // create scene
    sceneRef = make_shared<ofxInterface::Node>();
    sceneRef->setSize(ofGetWidth(), ofGetHeight());
    sceneRef->setName("ofxUiEditor-example-scene");

    layoutNode = NULL;
    loadLayouts("panel.frame");

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
    
    if(bCamEnabled)
        cam.begin();

    if(bDrawManager)
        meshDataManager.draw();

    sceneRef->render();
    
    if(bDrawDebug)
        sceneRef->renderDebug();
    
    if(bCamEnabled)
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

    if(key=='/'){
        bCamEnabled = !bCamEnabled;

        if(layoutNode && sceneRef->haveChild(layoutNode)){
            sceneRef->invertY(*layoutNode);
            layoutNode->setPosition(0,0,0);
        }

        return;
    }
}

void ofApp::exit(ofEventArgs &args){
    // remove our layoutNode so it doesn't get auto-deallocated
    // when our sceneRef instance goes out of scope
    if(layoutNode){
        sceneRef->removeChild(layoutNode);
    }

    // saveLayouts();
}

bool ofApp::loadLayouts(const string& createSceneNode){
    // clear scene
    while(sceneRef->getNumChildren() > 0)
        sceneRef->removeChild(0);

    // register instantiator method that maps layout info to component classes
    nodeGenerator.setInstantiator([this](shared_ptr<ofxUiEditor::MeshData> meshData) -> shared_ptr<ofxInterface::Node> {
        string typ = meshData->getType();
        
        if(typ == "AbortButton"){
            auto n = make_shared<AbortButton>();
            n->setup("Abort");
            return n;
        }
        
        if(typ == "SubmitButton"){
            auto n = make_shared<SubmitButton>();
            n->setup("Submit");
            return n;
        }
        
        // default
        return make_shared<ofxInterface::Node>();
    });

    ofLog() << "Loading layouts from " << layoutFile;
    if(!meshDataManager.loadFromFile(layoutFile))
        return false;

    if(createSceneNode != ""){
        auto nodeRef = nodeGenerator.generateNode(createSceneNode);

        if(nodeRef == nullptr){
            // loading the file succeeded, so we're still returning true
            return true;
        }

        // remove current layoutNode
        if(layoutNode)
            delete layoutNode;
        
        layoutNode = nodeRef.get();

        if(!layoutNode){
            ofLogWarning() << "Could not generate layout node with id: " << createSceneNode;
        } else {
            // layoutNode->setPosition(0,0,0);
            sceneRef->addChild(layoutNode);
            // not using 3d renderer, but 2d renderer?
            // flip Y axis
            if(!bCamEnabled){
                sceneRef->invertY(*layoutNode);
                layoutNode->setPosition(0,0,0);
            }
        }
    }
    return true;
}

bool ofApp::saveLayouts(){
    ofLog() << "Saving layouts to " << layoutFile;
    return meshDataManager.saveToFile(layoutFile);
}

void ofApp::onGeneratedNodeUpdated(ofxInterface::Node& node){
    // only invert when NOT using ofEasyCam
    if(bCamEnabled)
        return;

    auto pParent = (ofxInterface::Node*)node.getParent();
    if(!pParent)
        return;

    pParent->invertY(node, false /* not recursive */);
    
    if(&node == layoutNode)
        layoutNode->setPosition(0,0,0);
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
