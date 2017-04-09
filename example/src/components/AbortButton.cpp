#include "AbortButton.h"

void AbortButton::setup(const string& _label){
    ofxInterface::BitmapTextButton::setup(_label);
    setBGColor(ofColor(255,100,100));
}

void AbortButton::draw(){
    ofxInterface::BitmapTextButton::draw();
}
