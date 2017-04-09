#pragma once
// OF
#include "ofMain.h"
#include "ofxInterface.h"

class SubmitButton : public ofxInterface::BitmapTextButton {
    
public:
    void setup(const string& _label);
    void draw() override;
};
