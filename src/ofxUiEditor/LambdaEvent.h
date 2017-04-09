#pragma once

#include "ofEvent.h" // ofEvents
#include "ofxLiquidEvent.h" // lambda event

namespace ofxUiEditor {

    template<typename Type>
    class LambdaEvent : public ofEvent<Type>, ofxLiquidEvent<Type>  {

    public: // ofEvent
        inline void notify(const void* sender, Type & param);

    public: // ofxLiquidEvent methods
        void notifyListeners();
        void notifyListeners(Type& arguments);
        void notifyListenersInReverse(Type& arguments);
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
