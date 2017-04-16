#pragma once

// OF / addons
#include "ofMain.h"
#include "ofxInterface.h"
// local
#include "PropertiesManager.h" // for PropertiesItem class

namespace ofxUiEditor {
    namespace PropertiesActuators {
        
        void actuateSolidColorPanel(shared_ptr<ofxInterface::SolidColorPanel> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef);
        void actuateBitmapTextButton(shared_ptr<ofxInterface::BitmapTextButton> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef);

    };
}
