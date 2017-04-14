// ofxaddons
#include "ofxJSON.h"
// local
#include "PropertiesManager.h"

using namespace ofxUiEditor;

void PropertiesManager::setup(const string& filename){
    this->filename = filename;
    items.clear();
    load();
}

shared_ptr<PropertiesItem> PropertiesManager::get(const string& nodePath){
    for(auto item : items)
        if(item->getId() == nodePath)
            return item;
    return nullptr;
}

bool PropertiesManager::load(){
    ofxJSONElement json;

    if(!json.open(filename)){
        ofLogWarning() << "Could not load file: " << filename;
        return false;
    }

    // loop over each item in the josn
    for(auto it=json.begin(); it!=json.end(); it++){
        // create item instance and set id
        auto itemRef = make_shared<PropertiesItem>();
        itemRef->id = it.key().asString();

        // read all properties for this item
        auto json2 = json[itemRef->id];
        for(auto it2=json2.begin(); it2!=json2.end(); it2++){
            string key = it2.key().asString();
            itemRef->props[key] = json2[key].asString();
        }

        items.push_back(itemRef);
    }

    return true;
}
