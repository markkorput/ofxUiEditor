#pragma once

// OF
#include "ofEvent.h"
#include "ofEventUtils.h"
// lambda event
#include "ofxLiquidEvent.h"

namespace ofxUiEditor {

    template<typename Type>
    class LambdaEvent : public ofEvent<Type>, ofxLiquidEvent<Type>  {

    public: // ofEvent method
        inline void notify(const void* sender, Type & param);

    public: // ofxLiquidEvent methods
        void notifyListeners();
        void notifyListeners(Type& arguments);
        void notifyListenersInReverse(Type& arguments);
        
    public: // custom methods
        void forward(ofEvent<Type> &event);
        void forwardTo(ofEvent<Type> &event);

        void onForwardSource(Type &arg);

    private: // attributes
        std::vector<ofEvent<Type>> forwardToEvents;
        std::vector<ofEvent<Type>> forwardFromEvents;
    };
}

using namespace ofxUiEditor;

// overwrite ofEvent method; call original implementation AND call lambda notifications
template<typename Type>
inline void LambdaEvent<Type>::notify(const void* sender, Type & param){
    ofEvent<Type>::notify(sender, param);
    ofxLiquidEvent<Type>::notifyListeners(param);
}

// overwrite ofxLiquidEvent method; call original implementation AND call ofEvent notifications
template<typename Type>
void LambdaEvent<Type>::notifyListeners(Type& arguments) {
    ofxLiquidEvent<Type>::notifyListeners(arguments);
    ofEvent<Type>::notify(arguments);
}

// overwrite ofxLiquidEvent method; call original implementation AND call ofEvent notifications
template<typename Type>
void LambdaEvent<Type>::notifyListeners() {
    ofxLiquidEvent<Type>::notifyListeners();
    ofEvent<Type>::notify();
}

// overwrite ofxLiquidEvent method; call original implementation AND call ofEvent notifications
template<typename Type>
void LambdaEvent<Type>::notifyListenersInReverse(Type& arguments) {
    ofxLiquidEvent<Type>::notifyListenersInReverse(arguments);
    ofEvent<Type>::notify(arguments);
}

template<typename Type>
void LambdaEvent<Type>::forwardTo(ofEvent<Type> &event){
    forwardToEvents.push_back(event);
}

template<typename Type>
void LambdaEvent<Type>::forward(ofEvent<Type> &event){
    // register listener function
    ofAddListener(event, this, &LambdaEvent<Type>::onForwardSource);
    // keep a record of events we're following
    forwardFromEvents.push_back(event);
}

template<typename Type>
void LambdaEvent<Type>::onForwardSource(Type &arg){
    notifyListeners(arg);
}
