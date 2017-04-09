#include "SubmitButton.h"

void SubmitButton::setup(const string& _label){
    ofxInterface::BitmapTextButton::setup(_label);
    setBGColor(ofColor(100,255,100));
}

void SubmitButton::draw(){
    ofxInterface::BitmapTextButton::draw();
}
