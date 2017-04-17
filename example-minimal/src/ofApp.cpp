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
    editor.addType(".SolidColorPanel", // classname of the components to apply this type to
        OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::SolidColorPanel), // instantiator; class that manages this components
        ofxUiEditor::PropertiesActuators::actuateSolidColorPanel); // actuator; method that applies custom properties

    editor.addType(".BitmapTextButton",
        OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::BitmapTextButton),
        ofxUiEditor::PropertiesActuators::actuateBitmapTextButton);

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
