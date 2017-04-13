#include "StructureManager.h"

using namespace ofxUiEditor;

void StructureManager::setup(const string& filename){
    this->filename = filename;
    xmlRef = load();
}

shared_ptr<StructureInfo> StructureManager::get(const string& nodePath){
    auto info = make_shared<StructureInfo>();
    auto el = xmlRef->getPocoElement(nodePath);
    info->name = el->tagName();
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
