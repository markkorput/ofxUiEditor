#pragma once

#include "ofMain.h"
#include "PropertiesItem.h"

namespace ofxUiEditor {

    class PropertiesManager {

    public:
        bool load(const string& filename);
        shared_ptr<PropertiesItem> get(const string& id, bool create=true);
        void add(shared_ptr<PropertiesItem> itemRef);

    private:
        bool load();

    private:
        string filename;
        vector<shared_ptr<PropertiesItem>> items;
    };
}
