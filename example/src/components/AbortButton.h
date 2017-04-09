#pragma once
// OF
#include "ofMain.h"
#include "ofxInterface.h"

class AbortButton : public ofxInterface::BitmapTextButton {

public:
    void setup(const string& _label);

    void draw() override;
};
