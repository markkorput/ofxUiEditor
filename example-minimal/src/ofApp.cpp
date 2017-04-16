//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxUiEditor.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void draw();

private: // attributes
    ofxUiEditor::Editor<ofxInterface::Node> uiEditor;
    shared_ptr<ofxInterface::Node> sceneRef;
};


//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example-simple-layout");

    // create our root scene node, see ofxInterface
    sceneRef = make_shared<Node>();
    sceneRef->setSize(ofGetWidth(), ofGetHeight());
    sceneRef->setName("Scene");

    // load our editor and create a node
    uiEditor.setup(); // loads default data files, see structures.xml and properties.json
    auto windowNodeRef = uiEditor.create("window");

    sceneRef->addChild(windowNodeRef.get()); // ofxInterface doesn't use shared_ptr but regular pointers
    windowNodeRef->setCentered();
}


void ofApp::draw(){
    // render like normal, see ofxInterface
    sceneRef->render();
    sceneRef->renderDebug();
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
