#include "PropertiesItem.h"

using namespace ofxUiEditor;


void PropertiesItem::set(const string& key, const string& value){
    auto iterator = props.find(key);

    if(iterator == props.end()){ // no existing value
        props[key] = value;
        PropertyChange propChange(this, key);
        ofNotifyEvent(newPropertyEvent, propChange);
        return;
    }

    string previous = iterator->second;
    props[key] = value;
    if(value != previous){
        PropertyChange propChange(this, key);
        ofNotifyEvent(changeEvent, propChange);
    }
}

void PropertiesItem::merge(const PropertiesItem &other){
    if(other.getId() != id)
        ofLogWarning() << "PropertiesItem::merge - merging item with different id (" << other.getId() << " --> " << id << ")";

    auto otherProps = other.getProperties();
    for(auto it = otherProps.begin(); it != otherProps.end(); it++)
        set(it->first, it->second);
}

bool PropertiesItem::has(const string& key) const {
    return props.find(key) != props.end();
}

const string& PropertiesItem::get(const string& key, const string& defaultValue) const {
    auto it = props.find(key);
    if(it == props.end())
        return defaultValue;
    return it->second;
}

int PropertiesItem::get(const string &key, int defaultValue) const {
    auto it = props.find(key);
    if(it == props.end())
        return defaultValue;
    return ofToInt(it->second);
}

float PropertiesItem::get(const string &key, float defaultValue) const {
    auto it = props.find(key);
    if(it == props.end())
        return defaultValue;
    return ofToFloat(it->second);
}

bool PropertiesItem::hasVec2f(const string &key) const {
    return has(key+"_x") || has(key+"_y");
}

ofVec2f PropertiesItem::get(const string &key, const ofVec2f &defaultValue) const {
    return ofVec2f(get(key+"_x", defaultValue.x),
                   get(key+"_y", defaultValue.y));
}

bool PropertiesItem::hasVec3f(const string &key) const {
    return has(key+"_x") || has(key+"_y") || has(key+"_z");
}

ofVec3f PropertiesItem::get(const string &key, const ofVec3f &defaultValue) const {
    return ofVec3f(get(key+"_x", defaultValue.x),
                   get(key+"_y", defaultValue.y),
                   get(key+"_z", defaultValue.z));
}

ofVec4f PropertiesItem::get(const string &key, const ofVec4f &defaultValue) const {
    return ofVec4f(get(key+"_r", defaultValue.x),
                   get(key+"_g", defaultValue.y),
                   get(key+"_b", defaultValue.z),
                   get(key+"_a", defaultValue.w));
}

bool PropertiesItem::hasColor(const string &key) const {
    return has(key+"_r") || has(key+"_g") || has(key+"_b") || has(key+"_a");
}

ofColor PropertiesItem::get(const string &key, const ofColor &defaultValue) const {
    return ofColor(get(key+"_r", defaultValue.r),
                   get(key+"_g", defaultValue.g),
                   get(key+"_b", defaultValue.b),
                   get(key+"_a", defaultValue.a));
}
