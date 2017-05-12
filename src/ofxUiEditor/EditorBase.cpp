#include "EditorBase.h"

using namespace ofxUiEditor;

void EditorBase::setup(){
    if(!structureManager.isLoaded() && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_STRUCTURE_FILE))
        structureManager.setup(OFXUIEDITOR_DEFAULT_STRUCTURE_FILE);
}

shared_ptr<NodeModel> EditorBase::get(const string& nodePath, bool recursive){
    auto nodeModelRef = make_shared<NodeModel>();
    nodeModelRef->set("id", nodePath);

    // try to find structure information
    {
        auto infoRef = structureManager.get(nodePath);
        if(!infoRef){
            ofLogWarning() << "no structure infoRef found for nodePath: " << nodePath;
            return nodeModelRef;
        }

        nodeModelRef->set("class", infoRef->getClass());

        if(recursive){
            const vector<string>& childNames = infoRef->getChildNames();
            for(auto& childName : childNames){
                nodeModelRef->addChild(get(nodePath + StructureManager::SEPARATOR + childName, recursive));
            }
        }
    }
    return nodeModelRef;
}

void EditorBase::reload(){
    ofLogWarning() << "TODO: reload structureManager?"; // First have structure manager work ofxCMS::Collection-based as well?
}
