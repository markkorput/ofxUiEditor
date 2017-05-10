#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class StructureInfo {
    public:
        const string& getId(){ return id; }
        const string& getName(){ return name; }
        const string& getClass(){ return className; }
        const vector<string>& getChildNames(){ return childNames; }

    public:
        string id, name, className;
        vector<string> childNames;
    };

    class StructureManager {

    public:
        static string SEPARATOR;

    public:
        StructureManager() : filename(""), xmlRef(nullptr){}
        void setup(const string& filename);
        shared_ptr<StructureInfo> get(const string& nodePath);

        bool isLoaded(){ return xmlRef != nullptr; }

    private:
        shared_ptr<ofXml> load();

    private:
        string filename;
        shared_ptr<ofXml> xmlRef;
    };
}
