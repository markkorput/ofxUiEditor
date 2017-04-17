#include "ofxUnitTests.h"
#include "ofxUiEditor.h"

class CustomProgressBar : public ofxInterface::Node {
public: // static methods

    static void actuateProperties(shared_ptr<CustomProgressBar> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        nodeRef->emptyColor = propertiesRef->get("empty-color", ofColor::black);
        nodeRef->fullColor = propertiesRef->get("full-color", ofColor::white);
    }

public: // attribute
    float progress;
    ofColor emptyColor, fullColor;

public:
    CustomProgressBar() : progress(0.0f), emptyColor(ofColor::black), fullColor(ofColor::white){
    }
};

class ofApp: public ofxUnitTestsApp{
    void run(){
        // Create our editor instance with the standard
        // ofxInterface::Node as base Node type
        ofxUiEditor::Editor<ofxInterface::Node> editor;
        editor.setup();

        // We're using custom classes for component behaviour, so we need to register
        // an instantiator callbacks the know how to instantiate the types of nodes
        editor.addInstantiator(".MyProgressBar", []() -> shared_ptr<ofxInterface::Node> {
            return make_shared<CustomProgressBar>();
        });

        {   ofLog() << "Structure";
            // verify node structure
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
        }

        {   ofLog() << "Default property actuation";
            // verify properties are initialized
            shared_ptr<ofxInterface::Node> nodeRef = editor.create("window");
            test_eq(nodeRef->getSize(), ofVec2f(300.0f, 200.0f), "");
            test_eq(nodeRef->getPosition(), ofVec3f(123.0f, 456.0f, 789.0f), "");
            test_eq(nodeRef->getScale(), ofVec3f(0.5f, .25f, .1f), "");
        }

        {   ofLog() << "Custom property defaults";
            // verify custom properties are NOT properly actuated
            // because we haven't registered a custom properties actuator
            // for these properties yet.
            auto nodeRef = editor.create("popupDialog");
            auto progressBar = (CustomProgressBar*)nodeRef->getChildWithName(
                "CustomProgressBar");
            test_eq(progressBar->getName(), "CustomProgressBar", "");
            test_eq(progressBar->getSize(), ofVec2f(420, 25), ""); // size IS default property
            test_eq(progressBar->fullColor, ofColor::white, ""); // custom
            test_eq(progressBar->emptyColor, ofColor::black, ""); // custom
        }

        // register our custom properties actuator
        editor.addComponentPropertiesActuator(".MyProgressBar", [](shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
            CustomProgressBar::actuateProperties(static_pointer_cast<CustomProgressBar>(nodeRef), propertiesRef);
        });

        auto progressBarRef = static_pointer_cast<CustomProgressBar>(
            editor.create("popupDialog/CustomProgressBar"));

        {   ofLog() << "Custom properties actuation";
            test_eq(progressBarRef->getSize(), ofVec2f(420.0f, 25.0f), "");
            test_eq(progressBarRef->fullColor, ofColor(0, 200, 0), "");
            test_eq(progressBarRef->emptyColor, ofColor(140,130,130), "");
        }

        {   ofLog() << "Reload";
            // replace properties.json
            ofFile::moveFromTo("properties.json", "properties.json.bak");
            ofFile::moveFromTo("properties2.json", "properties.json");

            // before reload
            test_eq(progressBarRef->getSize(), ofVec2f(420.0f, 25.0f), "");
            // reload
            editor.reload();
            // after
            test_eq(progressBarRef->getSize(), ofVec2f(440.0f, 30.0f), "");

            // restore original properties.json
            ofFile::moveFromTo("properties.json", "properties2.json");
            ofFile::moveFromTo("properties.json.bak", "properties.json");
        }

        {   ofLog() << "Cleanup";
            test_eq(progressBarRef.use_count() > 1, true, "");
            editor.remove(progressBarRef);
            test_eq(progressBarRef.use_count(), 1, "");
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
