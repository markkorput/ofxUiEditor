#pragma once

// OF/addons
#include "ofMain.h"
// local/ofxUiEditor
#include "LambdaEvent.h"

namespace ofxUiEditor {

    class PropertiesItem;

    struct PropertyChange{
        PropertyChange(){}
        PropertyChange(PropertiesItem* item, const string& k) : propertiesItem(item), key(k){}
        PropertiesItem* propertiesItem;
        string key;
    };

    class PropertiesItem {

    public: // methods
        void setId(const string& newId){ id = newId; }
        void set(const string& key, const string& value);
        void merge(const PropertiesItem &other);

        string getId() const { return id; }
        const map<string, string>& getProperties() const { return props; }

        const string& get(const string& key, const string& defaultValue) const;
        int get(const string &key, int defaultValue) const;
        float get(const string &key, float defaultValue) const;
        ofVec2f get(const string &key, const ofVec2f &defaultValue) const;
        ofVec3f get(const string &key, const ofVec3f &defaultValue) const;
        ofVec4f get(const string &key, const ofVec4f &defaultValue) const;
        ofColor get(const string &key, const ofColor &defaultValue) const;

        bool has(const string& key) const;
        bool hasVec2f(const string &key) const;
        bool hasVec3f(const string &key) const;
        bool hasColor(const string &key) const;

    public: // events
        ofEvent<PropertyChange> newPropertyEvent;
        ofEvent<PropertyChange> propertyChangeEvent;
        LambdaEvent<PropertiesItem> changeEvent;

    private: // attributes
        string id;
        std::map<string, string> props;
    };
}
