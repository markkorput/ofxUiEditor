#include "ofxUnitTests.h"
#include "ofxUiEditor/EditorOld.h"
#include "ofxUiEditor.h"

#define TEST_START(x) {ofLog()<<"CASE: "<<#x;
#define TEST_END }

class CustomProgressBar : public ofxInterface::Node {
public: // static methods

    static void actuateProperties(shared_ptr<ofxInterface::Node> nRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        ofxUiEditor::PropertiesActuators::actuateNode(nRef, propertiesRef);
        auto nodeRef = static_pointer_cast<CustomProgressBar>(nRef);
        nodeRef->emptyColor = propertiesRef->get("empty-color", ofColor::black);
        nodeRef->fullColor = propertiesRef->get("full-color", ofColor::white);
    }

public: // attribute
    float progress;
    ofColor emptyColor, fullColor;

public:
    CustomProgressBar() : progress(0.0f), emptyColor(ofColor::black), fullColor(ofColor::white){
        ofLog() << "CONSTRUCTOR";
    }
};

class ofApp: public ofxUnitTestsApp{
    void runEditorOld(){
        // Create our editor instance with the standard
        // ofxInterface::Node as base Node type
        ofxUiEditor::EditorOld<ofxInterface::Node> editor;
        editor.setup();

        // We're using custom classes for component behaviour, so we need to register
        // an instantiator callbacks the know how to instantiate the types of nodes
        editor.addInstantiator(".MyProgressBar", OFX_UI_EDITOR_INSTANTIATOR(CustomProgressBar));

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

        // {   ofLog() << "Custom property defaults";
        //     // verify custom properties are NOT properly actuated
        //     // because we haven't registered a custom properties actuator
        //     // for these properties yet.
        //     test_eq(progressBarRef->getName(), "CustomProgressBar", "");
        //     test_eq(progressBarRef->getSize(), ofVec2f(420, 25), ""); // size IS default property
        //     test_eq(progressBarRef->fullColor, ofColor::white, ""); // custom
        //     test_eq(progressBarRef->emptyColor, ofColor::black, ""); // custom
        // }

        // register our custom properties actuator
        editor.addActuator(".MyProgressBar", CustomProgressBar::actuateProperties);

        auto progressBarRef = static_pointer_cast<CustomProgressBar>(editor.create("popupDialog/CustomProgressBar"));

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

        {   ofLog() << "operator[]";
            test_eq((CustomProgressBar*)editor["CustomProgressBar"]->getCurrent(), progressBarRef.get(), "");
        }
    }

    void runManager(){
        TEST_START(get layout data)
            ofxUiEditor::Manager<ofxInterface::Node> man;
            man.setup();
            shared_ptr<NodeModel> modelRef = man.get("window");

            // structure
            test_eq(modelRef->getId(), "window", "");
            test_eq(modelRef->getClass(), "SolidColorPanel", "");
            test_eq(modelRef->getParent() == nullptr, true, "");
            test_eq(modelRef->getChildren().size(), 4, "");
            test_eq(modelRef->getChildren()[0]->getId(), "window/titlebar", "");
            test_eq(modelRef->getChildren()[0]->getName(), "titlebar", "");
            test_eq(modelRef->getChildren()[0]->getClass(), "SolidColorPanel", "");
            test_eq(modelRef->getChildren()[0]->getChildren().size(), 2, "");

            test_eq(modelRef->getChildren()[0]->getChildren()[0]->getId(), "window/titlebar/title", "");
            test_eq(modelRef->getChildren()[0]->getChildren()[0]->getName(), "title", "");
            test_eq(modelRef->getChildren()[0]->getChildren()[0]->getClass(), "BitmapTextButton", "");

            test_eq(modelRef->getChildren()[0]->getChildren()[1]->getId(), "window/titlebar/close", "");
            test_eq(modelRef->getChildren()[0]->getChildren()[1]->getName(), "close", "");
            test_eq(modelRef->getChildren()[0]->getChildren()[1]->getClass(), "BitmapTextButton", "");

            test_eq(modelRef->getChildren()[1]->getId(), "window/message", "");
            test_eq(modelRef->getChildren()[1]->getName(), "message", "");
            test_eq(modelRef->getChildren()[1]->getClass(), "BitmapTextButton", "");

            test_eq(modelRef->getChildren()[2]->getId(), "window/cancel", "");
            test_eq(modelRef->getChildren()[2]->getName(), "cancel", "");
            test_eq(modelRef->getChildren()[2]->getClass(), "BitmapTextButton", "");

            test_eq(modelRef->getChildren()[3]->getId(), "window/submit", "");
            test_eq(modelRef->getChildren()[3]->getName(), "submit", "");
            test_eq(modelRef->getChildren()[3]->getClass(), "BitmapTextButton", "");
        TEST_END

        TEST_START(Instantiate base-type nodes)
            ofxUiEditor::Manager<ofxInterface::Node> man;
            man.setup();
            shared_ptr<ofxInterface::Node> nodeRef = man.instantiate("window");
            test_eq(nodeRef->getNumChildren(), 4, "");
            test_eq(nodeRef->getName(), "window", "");
            test_eq(nodeRef->getChildren()[0]->getName(), "titlebar", "");
            test_eq(nodeRef->getChildren()[0]->getNumChildren(), 2, "");
            test_eq(nodeRef->getChildren()[0]->getChildren()[0]->getName(), "title", "");
            test_eq(nodeRef->getChildren()[0]->getChildren()[1]->getName(), "close", "");
            test_eq(nodeRef->getChildren()[1]->getName(), "message", "");
            test_eq(nodeRef->getChildren()[2]->getName(), "cancel", "");
            test_eq(nodeRef->getChildren()[3]->getName(), "submit", "");
        TEST_END

        TEST_START(Instantiate custom node types)
            class HierarchyAnalyserNode : public ofxInterface::Node {
                public:
                    const string& getMyType(){ return customTypeValue; }

                private:
                    string customTypeValue = "I'm a HierarchyAnalyserNode";
            };

            ofxUiEditor::Manager<ofxInterface::Node> man;
            man.setup();

            // add id-based instantiator
            man.addInstantiator("window", [](shared_ptr<ofxUiEditor::NodeModel> nodeModel){
                return make_shared<HierarchyAnalyserNode>();
            });

            auto nodeRef = man.instantiate("window");
            test_eq(static_pointer_cast<HierarchyAnalyserNode>(nodeRef)->getMyType(), "I'm a HierarchyAnalyserNode", "");
        }

        TEST_START(Actuate properties from properties.json)
            ofxUiEditor::Manager<ofxInterface::Node> man;
            man.setup();
            auto nodeRef = man.instantiate("window");
            test_eq(nodeRef->getSize(), ofVec2f(300,200), "");
            test_eq(nodeRef->getPosition(), ofVec3f(123,456,789), "");
            test_eq(nodeRef->getScale(), ofVec3f(0.5,0.25,0.1), "");
        TEST_END
    }

    void run(){
        runManager();
        // runEditorOld();
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
