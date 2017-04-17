//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxUiEditor.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void onLeftClick(ofxInterface::TouchEvent& touchEvent);

private: // attributes
    ofxInterface::TouchManager touchManager;
    ofxUiEditor::Editor<ofxInterface::Node> editor;
    shared_ptr<ofxInterface::Node> sceneRef;
    bool bShowDebug;
};


//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    // window
    ofSetWindowTitle("ofxUiEditor - example-animation");
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
    
    auto left = nodeRef->getChildWithName("left");
    left->setCenteredV();
    left->setX(nodeRef->getWidth()*0.5f - left->getWidth() - 5);
    auto right = nodeRef->getChildWithName("right");
    right->setCenteredV();
    right->setX(nodeRef->getWidth()*0.5f+5);

    touchManager.setup(sceneRef.get());
    
    ofAddListener(left->eventClick, this, &ofApp::onLeftClick);
}

void ofApp::update(){
    touchManager.update();
}

void ofApp::draw(){
    ofBackground(0);

    sceneRef->render(); // render like normal, see ofxInterface

    if(bShowDebug){
        sceneRef->renderDebug();
        touchManager.draw();
    }
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

void ofApp::mouseDragged(int x, int y, int button){
    ofxInterface::TouchManager::one().touchMove(button, ofVec2f(x, y));
}

void ofApp::mousePressed(int x, int y, int button){
    ofxInterface::TouchManager::one().touchDown(button, ofVec2f(x, y));
}

void ofApp::mouseReleased(int x, int y, int button){
    ofxInterface::TouchManager::one().touchUp(button, ofVec2f(x, y));
}

void ofApp::onLeftClick(ofxInterface::TouchEvent& touchEvent){
    // start animation "left" on the "object" node
    editor["object"].animate("left");
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main(){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
