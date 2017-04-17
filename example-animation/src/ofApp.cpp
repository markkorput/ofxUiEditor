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
    void keyPressed(int key);

private: // attributes
    ofxUiEditor::Editor<ofxInterface::Node> editor;
    shared_ptr<ofxInterface::Node> sceneRef;
    bool bShowDebug;
};


//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example-simple-layout");
    bShowDebug = false;

    // setup our editor and register component types
    editor.setup(); // loads default data files, see structures.xml and properties.json

    // create our root scene node, see ofxInterface
    sceneRef = make_shared<ofxInterface::Node>();
    sceneRef->setSize(ofGetWidth(), ofGetHeight());
    sceneRef->setName("Scene");

    // populate our scene
    auto nodeRef = editor.create("object");
    sceneRef->addChild(nodeRef.get()); // ofxInterface doesn't use shared_ptr but regular pointers
    nodeRef->setCentered();
    
    nodeRef->getChildWithName("left")->setCenteredV();
    nodeRef->getChildWithName("left")->setX(nodeRef->getWidth()*0.5f-nodeRef->getChildWithName("left")->getWidth()-5);
    nodeRef->getChildWithName("right")->setCenteredV();
    nodeRef->getChildWithName("right")->setX(nodeRef->getWidth()*0.5f+5);
}


void ofApp::draw(){
    ofBackground(0);

    sceneRef->render(); // render like normal, see ofxInterface

    if(bShowDebug)
        sceneRef->renderDebug();
}

void ofApp::keyPressed(int key){
    switch (key) {
    case 'd':
        bShowDebug = !bShowDebug;
        return;

    case 'r':
        sceneRef->setSize(ofGetWidth(), ofGetHeight());
        editor.reload();

        if(auto windowNodePtr = sceneRef->getChildWithName("window"))
            windowNodePtr->setCentered();

        return;
    }
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
