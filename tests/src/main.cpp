#include "ofxUnitTests.h"
#include "../../src/ofxUiEditor.h"

class ofApp: public ofxUnitTestsApp{
    void run(){
        // StructureManager
        string filename = "structures.xml";
        ofxUiEditor::StructureManager strucman;
        strucman.setup(filename);

        // TODO; StructureManager unit-tests?

        // Editor with StructureManager
        ofxUiEditor::Editor editor;
        editor.use(strucman);
        shared_ptr<ofxInterface::Node> nodeRef = editor.create("window");
        test_eq(nodeRef->getName(), "window", "name");
        auto& children =  nodeRef->getChildren();
        test_eq(children.get(0)->getName(), "titlebar");
        test_eq(children.get(0)->getChildren().get(0)->getName(), "title");
        test_eq(children.get(0)->getChildren().get(1)->getName(), "close");
        test_eq(children.get(1)->getName(), "message");
        test_eq(children.get(2)->getName(), "cancel");
        test_eq(children.get(3)->getName(), "submit");
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
