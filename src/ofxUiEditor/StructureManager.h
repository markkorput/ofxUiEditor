#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class StructureInfo {
    public:
        string getName(){ return name; }
        const vector<string>& getChildNames(){ return childNames; }

    public:
        string name;
        vector<string> childNames;
    };

    class StructureManager {

    public:
        static string SEPARATOR;

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
