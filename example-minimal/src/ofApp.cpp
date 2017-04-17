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
    editor.addInstantiator(".SolidColorPanel", OFX_UI_INSTANTIATOR(ofxInterface::SolidColorPanel));
    editor.addInstantiator(".BitmapTextButton", OFX_UI_INSTANTIATOR(ofxInterface::BitmapTextButton));
    editor.addActuator(".SolidColorPanel", ofxUiEditor::PropertiesActuators::actuateSolidColorPanel);
    editor.addActuator(".BitmapTextButton", ofxUiEditor::PropertiesActuators::actuateBitmapTextButton);

    // create our root scene node, see ofxInterface
    sceneRef = make_shared<ofxInterface::Node>();
    sceneRef->setSize(ofGetWidth(), ofGetHeight());
    sceneRef->setName("Scene");

    // populate our scene
    auto windowNodeRef = editor.create("window");
    sceneRef->addChild(windowNodeRef.get()); // ofxInterface doesn't use shared_ptr but regular pointers
    windowNodeRef->setCentered();
}


void ofApp::draw(){
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

        if(auto windowNodePtr = sceneRef->getChildWithName("window")){
            
            windowNodePtr->setCentered();
        }
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
