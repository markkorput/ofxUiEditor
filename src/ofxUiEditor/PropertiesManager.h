#pragma once

#include "ofMain.h"

namespace ofxUiEditor {

    class PropertiesItem;

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


    class PropertiesItem {
    public:
        string getId(){ return id; }
        const map<string, string>& getProperties(){ return props; }

        bool has(const string& key){
            return props.find(key) != props.end();
        }

        const string& get(const string& key, const string& defaultValue){
            auto it = props.find(key);
            if(it == props.end())
                return defaultValue;
            return it->second;
        }
    public:
        string id;
        map<string, string> props;
    };
}
