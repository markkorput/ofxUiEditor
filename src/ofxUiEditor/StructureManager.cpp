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
    info->name = el->tagName();

    auto childNodes = el->childNodes();
    for(int i=0; i<childNodes->length(); i++){
        auto item = childNodes->item(i);
        if(item->nodeType() == Poco::XML::Node::ELEMENT_NODE)
            info->childNames.push_back(childNodes->item(i)->nodeName());
    }

    return info;
}

shared_ptr<ofXml> StructureManager::load(){
    ofFile file;
    file.open(filename);
    ofBuffer buffer = file.readToBuffer();
    auto xml = make_shared<ofXml>();
    xml->loadFromBuffer( buffer.getText() );
    return xml;
}
