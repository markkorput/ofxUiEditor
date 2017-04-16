#include "ofxUnitTests.h"
#include "ofxUiEditor.h"

class CustomProgressBar : public ofxInterface::Node {
public: // attribute
    float progress;
    ofColor emptyColor, fullColor;

public:
    CustomProgressBar() : progress(0.0f), emptyColor(ofColor::black), fullColor(ofColor::white){
    }
};

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

        // start using custom components classes properties manager
        editor.use(propman);
        editor.addInstantiator("popupDialog/CustomProgressBar", []() -> shared_ptr<ofxInterface::Node> {
            return make_shared<CustomProgressBar>();
        });

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
            auto progressBar = (CustomProgressBar*)nodeRef->getChildWithName(
                "CustomProgressBar");
            test_eq(progressBar->getName(), "CustomProgressBar", "");
            test_eq(progressBar->getSize(), ofVec2f(420, 25), ""); // default property
            test_eq(progressBar->fullColor, ofColor::white, ""); // custom
            test_eq(progressBar->emptyColor, ofColor::black, ""); // custom
        }

        // register our custom properties actuator
        editor.addComponentPropertiesActuator("popupDialog/CustomProgressBar", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
            // our custom properties are obviously not part of the base class,
            // so we need to typecast the pointer
            auto progressBarRef = static_pointer_cast<CustomProgressBar>(nodeRef);
            progressBarRef->emptyColor = propertiesRef->get("empty-color", ofColor::black);
            progressBarRef->fullColor = propertiesRef->get("full-color", ofColor::white);
        });

        {   // verify custom properties ARE properly actuated
            auto progressBarRef = static_pointer_cast<CustomProgressBar>(
                editor.create("popupDialog/CustomProgressBar"));
            test_eq(progressBarRef->getSize(), ofVec2f(420.0f, 25.0f), "");
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
