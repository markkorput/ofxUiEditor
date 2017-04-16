#pragma once

#include "ofMain.h"
#include "ofxInterface.h"

namespace ofxUiEditor{
    class BaseNode : public ofxInterface::Node {

    private: // attributes
        bool deleteChildrenOnDestroy;

    public: // methods
        BaseNode() : deleteChildrenOnDestroy(false){}
        ~BaseNode();

        void setDeleteChildrenOnDestroy(bool enabled){
            deleteChildrenOnDestroy = enabled;
        }
    };
}
