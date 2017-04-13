#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class StructureInfo {
    public:
        string getName(){ return name; }

    public:
        string name;
        vector<shared_ptr<StructureInfo>> children;
    };

    class StructureManager {
    public:
        void setup(const string& filename);
        shared_ptr<StructureInfo> get(const string& nodePath);

    private:
        shared_ptr<ofXml> load();

    private:
        string filename;
        shared_ptr<ofXml> xmlRef;
    };
}
