#include "BaseNode.h"

using namespace ofxUiEditor;

BaseNode::~BaseNode(){
    // ofLogVerbose() << "~BaseNode";

    if(deleteChildrenOnDestroy)
        // notinhg to do
        return;

    // remove children before the ofxInterface::Node destrcutor tries to delete them
    while (!childNodes.empty())
        removeChild(0);
}
