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

        int get(const string &key, int defaultValue){
            std::map<string,string>::iterator it = props.find(key);
            if(it == props.end())
                return defaultValue;
            return ofToInt(it->second);
        }

        float get(const string &key, float defaultValue){
            std::map<string,string>::iterator it = props.find(key);
            if(it == props.end())
                return defaultValue;
            return ofToFloat(it->second);
        }

        ofVec2f get(const string &key, const ofVec2f &defaultValue){
            return ofVec2f(get(key+"_x", defaultValue.x),
                           get(key+"_y", defaultValue.y));
        }

        ofVec3f get(const string &key, const ofVec3f &defaultValue){
            return ofVec3f(get(key+"_x", defaultValue.x),
                           get(key+"_y", defaultValue.y),
                           get(key+"_z", defaultValue.z));
        }

        ofVec4f get(const string &key, const ofVec4f &defaultValue){
            return ofVec4f(get(key+"_r", defaultValue.x),
                           get(key+"_g", defaultValue.y),
                           get(key+"_b", defaultValue.z),
                           get(key+"_a", defaultValue.w));
        }

        ofColor get(const string &key, const ofColor &defaultValue){
            return ofColor(get(key+"_r", defaultValue.r),
                           get(key+"_g", defaultValue.g),
                           get(key+"_b", defaultValue.b),
                           get(key+"_a", defaultValue.a));
        }

    public:
        string id;
        std::map<string, string> props;
    };
}
