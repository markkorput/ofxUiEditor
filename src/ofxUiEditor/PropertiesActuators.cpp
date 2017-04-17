#include "PropertiesActuators.h"

namespace ofxUiEditor { namespace PropertiesActuators {

    void actuateNode(shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        nodeRef->setPosition(propertiesRef->get("position", ofPoint(0.0f)));
        nodeRef->setScale(propertiesRef->get("scale", ofVec3f(1.0f)));
        nodeRef->setOrientation(propertiesRef->get("orientation", ofVec3f(0.0f)));
        nodeRef->setSize(propertiesRef->get("size", ofVec2f(200.0f, 100.0f)));
    }

    void actuateSolidColorPanel(shared_ptr<ofxInterface::Node> nRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        actuateNode(nRef, propertiesRef);
        auto nodeRef = static_pointer_cast<ofxInterface::SolidColorPanel>(nRef);

        bool hasBG = propertiesRef->hasColor("background");
        if(hasBG)
            nodeRef->setBGColor(propertiesRef->get("background", ofColor(255)));
        nodeRef->setBackground(hasBG);

        bool hasBorder = propertiesRef->hasColor("border");
        if(hasBorder)
            nodeRef->setStrokeColor(propertiesRef->get("border", ofColor(0)));
        nodeRef->setBorder(hasBorder);
    }

    void actuateBitmapTextButton(shared_ptr<ofxInterface::Node> nRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        actuateNode(nRef, propertiesRef);
        auto nodeRef = static_pointer_cast<ofxInterface::BitmapTextButton>(nRef);

        if(propertiesRef->has("text"))
            nodeRef->setup(propertiesRef->get("text", ""));

        // the text property overwrites the size, so let's re-apply is specified
        if(propertiesRef->hasVec2f("size")){
            nodeRef->setSize(propertiesRef->get("size", nodeRef->getSize()));
        }

        bool hasBG = propertiesRef->hasColor("background");
        if(hasBG)
            nodeRef->setBGColor(propertiesRef->get("background", ofColor(255)));
        nodeRef->setBackground(hasBG);

        bool hasBorder = propertiesRef->hasColor("border");
        if(hasBorder)
            nodeRef->setBorderColor(propertiesRef->get("border", ofColor(0)));
        nodeRef->setBorder(hasBorder);

        bool hasColor = propertiesRef->hasColor("color");
        if(hasColor)
            nodeRef->setLabelColor(propertiesRef->get("color", ofColor(0)));
    }
}}
