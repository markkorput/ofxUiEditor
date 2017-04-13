#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class StructureManager {
    public:
        void setup(const string& filename);

    private:
        string filename;
    };
}
