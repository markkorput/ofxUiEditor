#pragma once

#include "ofMain.h"

namespace ofxUiEditor {
    class PropertiesItem {
    public:
        string getId(){ return id; }
        const map<string, string>& getProperties(){ return props; }

    public:
        string id;
        map<string, string> props;
    };

    class PropertiesManager {

    public:
        void setup(const string& filename);
        shared_ptr<PropertiesItem> get(const string& id);

    private:
        bool load();

    private:
        string filename;
        vector<shared_ptr<PropertiesItem>> items;
    };
}
