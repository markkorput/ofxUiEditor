#include "StructureManager.h"

using namespace ofxUiEditor;

string StructureManager::SEPARATOR = "/";

void StructureManager::setup(const string& filename){
    this->filename = filename;
    xmlRef = load();
}

shared_ptr<StructureInfo> StructureManager::get(const string& nodePath){
    auto info = make_shared<StructureInfo>();
    auto el = xmlRef->getPocoElement(nodePath);

    if(!el)
        return info;

    info->id = nodePath;
    info->name = el->tagName();
    info->className = el->getAttribute("class");

    ofLog() << "StructureManager; found class " << info->className << " for " << nodePath;
    auto childNodes = el->childNodes();
    for(int i=0; i<childNodes->length(); i++){
        auto item = childNodes->item(i);
        if(item->nodeType() == Poco::XML::Node::ELEMENT_NODE)
            info->childNames.push_back(childNodes->item(i)->nodeName());
    }

    return info;
}

shared_ptr<ofXml> StructureManager::load(){
    auto xml = make_shared<ofXml>();
    ofFile file;

    if(!file.open(filename)){
        ofLogWarning() << "ofxUiEditor::StructureManager - couldn't open file: " << filename;
        return xml;
    }

    ofBuffer buffer = file.readToBuffer();

    if(!xml->loadFromBuffer( buffer.getText() ))
        ofLogWarning() << "ofxUiEditor::StructureManager - could not load from file: " << filename;

    return xml;
}
