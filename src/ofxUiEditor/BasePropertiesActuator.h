#pragma once

#include "ofMain.h"
#include "PropertiesManager.h"

namespace ofxUiEditor {

    template<class NodeType>
    class BasePropertiesActuator {
    public:
        virtual void actuate(shared_ptr<NodeType> nodeRef, shared_ptr<PropertiesItem> propertiesRef);
    };
}

using namespace ofxUiEditor;

template<class NodeType>
void BasePropertiesActuator<NodeType>::actuate(
            shared_ptr<NodeType> nodeRef,
            shared_ptr<PropertiesItem> propertiesRef){
    nodeRef->setPosition(propertiesRef->get("position", ofPoint(0.0f)));
    nodeRef->setScale(propertiesRef->get("scale", ofVec3f(1.0f)));
    nodeRef->setOrientation(propertiesRef->get("orientation", ofVec3f(0.0f)));
    nodeRef->setSize(propertiesRef->get("position", ofVec2f(200.0f, 100.0f)));
}
