#include "ofxUnitTests.h"
#include "ofxUiEditor.h"

namespace CustomProgressBar{
    class Component : public ofxInterface::Node {

    public: // attribute
        float progress;
        ofColor emptyColor, fullColor;

    public:
        Component() : progress(0.0f), emptyColor(0.0f), fullColor(1.0f){
        }
    };

    class PropertiesActuator : public ofxUiEditor::BasePropertiesActuator<ofxInterface::Node> {
    public:
        void actuate(shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
            auto progressBarRef = (shared_ptr<Component>) nodeRef;
            // let our parent class take care of the default properties (size, position, scale, rotation)
            ofxUiEditor::BasePropertiesActuator<ofxInterface::Node>::actuate(progressBarRef, propertiesRef);
            progressBarRef->emptyColor = propertiesRef->get("empty-color", ofColor(0.0f));
            progressBarRef->fullColor = propertiesRef->get("full-color", ofColor(255.0f));
        }
    };
}

using namespace CustomProgressBar;

class ofApp: public ofxUnitTestsApp{
    void run(){
        // StructureManager
        ofxUiEditor::StructureManager strucman;
        strucman.setup("structures.xml");
        // TODO; StructureManager unit-tests?

        // Editor with StructureManager
        ofxUiEditor::Editor<ofxInterface::Node> editor;
        editor.use(strucman);

        {   // verify node structure
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

        {   // verify node structure
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
            test_eq(nodeRef->getPosition(), ofVec3f(123.0f, 456.0f, 789.0f), "");
            test_eq(nodeRef->getScale(), ofVec3f(0.5f, .25f, .1f), "");
        }

        {   // verify custom properties are NOT properly actuated
            auto nodeRef = editor.create("popupDialog");
            auto progressBar = (CustomProgressBar::Component*)nodeRef->getChildWithName(
                "CustomProgressBar");
            test_eq(progressBar->getWidth(), 0, "");
            test_eq(progressBar->getHeight(), 0, "");
            test_eq(progressBar->fullColor, ofColor(1.0f), "");
            test_eq(progressBar->emptyColor, ofColor(0.0f), "");
        }

        // register our custom properties actuator
        editor.addComponentPropertiesActuator("popupDialog/CustomProgressBar", make_shared<PropertiesActuator>());

        {   // verify custom properties ARE properly actuated
            auto progressBarRef = dynamic_pointer_cast<CustomProgressBar::Component>(
                editor.create("popupDialog/CustomProgressBar"));
            test_eq(progressBarRef->getWidth(), 420.0f, "");
            test_eq(progressBarRef->getHeight(), 25.0f, "");
            test_eq(progressBarRef->fullColor, ofColor(0, 200, 0), "");
            test_eq(progressBarRef->emptyColor, ofColor(140,130,130), "");
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
