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
    ofxUiEditor::Editor<ofxInterface::Node> editor;
    shared_ptr<ofxInterface::Node> sceneRef;
};


//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example-simple-layout");

    // load our editor and create a node
    editor.setup(); // loads default data files, see structures.xml and properties.json

    editor.addInstantiator("window", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::SolidColorPanel>();
    });

    editor.addInstantiator("window/titlebar", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::SolidColorPanel>();
    });

    editor.addInstantiator("window/titlebar/title", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::BitmapTextButton>();
    });

    editor.addInstantiator("window/titlebar/close", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::BitmapTextButton>();
    });

    editor.addInstantiator("window/message", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::BitmapTextButton>();
    });

    editor.addInstantiator("window/cancel", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::BitmapTextButton>();
    });

    editor.addInstantiator("window/submit", []() -> shared_ptr<ofxInterface::Node> {
        return make_shared<ofxInterface::BitmapTextButton>();
    });

    // register our custom properties actuator
    editor.addComponentPropertiesActuator("window", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateSolidColorPanel(static_pointer_cast<ofxInterface::SolidColorPanel>(nodeRef), propertiesRef);
    });

    editor.addComponentPropertiesActuator("window/titlebar", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateSolidColorPanel(static_pointer_cast<ofxInterface::SolidColorPanel>(nodeRef), propertiesRef);
    });

    editor.addComponentPropertiesActuator("window/titlebar/title", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateBitmapTextButton(static_pointer_cast<ofxInterface::BitmapTextButton>(nodeRef), propertiesRef);
    });

    editor.addComponentPropertiesActuator("window/titlebar/close", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateBitmapTextButton(static_pointer_cast<ofxInterface::BitmapTextButton>(nodeRef), propertiesRef);
    });

    editor.addComponentPropertiesActuator("window/message", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateBitmapTextButton(static_pointer_cast<ofxInterface::BitmapTextButton>(nodeRef), propertiesRef);
    });

    editor.addComponentPropertiesActuator("window/cancel", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateBitmapTextButton(static_pointer_cast<ofxInterface::BitmapTextButton>(nodeRef), propertiesRef);
    });

    editor.addComponentPropertiesActuator("window/submit", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateBitmapTextButton(static_pointer_cast<ofxInterface::BitmapTextButton>(nodeRef), propertiesRef);
    });

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
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
