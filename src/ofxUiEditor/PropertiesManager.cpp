// ofxaddons
#include "ofxJSON.h"
// local
#include "PropertiesManager.h"

using namespace ofxUiEditor;

bool PropertiesManager::load(const string& filename){
    ofxJSONElement json;

    if(!json.open(filename)){
        ofLogWarning() << "Could not load file: " << filename;
        return false;
    }

    // loop over each item in the josn
    for(auto it=json.begin(); it!=json.end(); it++){
        // create item instance and set id
        auto itemRef = make_shared<PropertiesItem>();
        itemRef->setId(it.key().asString());

        // read all properties for this item
        auto json2 = json[itemRef->getId()];
        for(auto it2=json2.begin(); it2!=json2.end(); it2++){
            string key = it2.key().asString();
            itemRef->set(key, json2[key].asString());
        }

        add(itemRef);
    }

    return true;
}

shared_ptr<PropertiesItem> PropertiesManager::get(const string& nodePath, bool create){
    // find existing
    for(auto item : items)
        if(item->getId() == nodePath)
            return item;

    // no existing item
    if(!create){
        ofLogVerbose() << "Could not find properties item for: " << nodePath;
        return nullptr;
    }

    // create new item
    ofLogVerbose() << "Creating empty properties item for: " << nodePath;
    auto item = make_shared<PropertiesItem>();
    item->setId(nodePath);
    items.push_back(item);
    return item;
}

void PropertiesManager::add(shared_ptr<PropertiesItem> itemRef){
    auto existingItemRef = get(itemRef->getId());
    if(existingItemRef){
        existingItemRef->merge(*itemRef);
        return;
    }

    items.push_back(itemRef);
}
