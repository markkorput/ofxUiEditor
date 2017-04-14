#include "ofxUnitTests.h"
#include "ofxUiEditor.h"

class ofApp: public ofxUnitTestsApp{
    void run(){
        // StructureManager
        ofxUiEditor::StructureManager strucman;
        strucman.setup("structures.xml");
        // TODO; StructureManager unit-tests?

        // Editor with StructureManager
        ofxUiEditor::Editor<ofxInterface::Node> editor;
        editor.use(strucman);

        // verify node structure
        {
            shared_ptr<ofxInterface::Node> nodeRef = editor.create("window");
            test_eq(nodeRef->getName(), "window", "");
            auto& children =  nodeRef->getChildren();
            test_eq(children.size(), 4, "");
            test_eq(children[0]->getName(), "titlebar", "");
            test_eq(children[0]->getChildren()[0]->getName(), "title", "");
            test_eq(children[0]->getChildren()[1]->getName(), "close", "");
            test_eq(children[1]->getName(), "message", "");
            test_eq(children[2]->getName(), "cancel", "");
            test_eq(children[3]->getName(), "submit", "");
            // verify properties not initialized
            test_eq(nodeRef->getSize(), ofVec2f(0.0f), "");
        }

        // PropertiesManager
        ofxUiEditor::PropertiesManager propman;
        propman.setup("properties.json");
        // TODO; PropertiesManager unit-tests?

        // start using properties manager
        editor.use(propman);

        // verify node structure
        {
            shared_ptr<ofxInterface::Node> nodeRef = editor.create("window");
            test_eq(nodeRef->getName(), "window", "");
            auto& children = nodeRef->getChildren();
            test_eq(children.size(), 4, "");
            test_eq(children[0]->getName(), "titlebar", "");
            test_eq(children[0]->getChildren()[0]->getName(), "title", "");
            test_eq(children[0]->getChildren()[1]->getName(), "close", "");
            test_eq(children[1]->getName(), "message", "");
            test_eq(children[2]->getName(), "cancel", "");
            test_eq(children[3]->getName(), "submit", "");
            // verify properties ARE initialized
            test_eq(nodeRef->getSize(), ofVec2f(300.0f, 200.0f), "");
        }
    }
};


#include "ofAppNoWindow.h"
#include "ofAppRunner.h"

//========================================================================
int main( ){
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
