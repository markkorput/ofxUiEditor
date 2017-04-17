#pragma once

// OF / addons
#include "ofMain.h"
#include "ofxInterface.h"
// local
#include "PropertiesItem.h"

namespace ofxUiEditor {
    namespace PropertiesActuators {

        void actuateNode(shared_ptr<ofxInterface::Node> nsRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef);
        void actuateSolidColorPanel(shared_ptr<ofxInterface::Node> nRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef);
        void actuateBitmapTextButton(shared_ptr<ofxInterface::Node> nRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef);

    };
}
