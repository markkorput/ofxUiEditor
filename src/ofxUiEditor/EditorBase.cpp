#include "EditorBase.h"

using namespace ofxUiEditor;

void EditorBase::setup(){
    if(!structureManager.isLoaded() && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_STRUCTURE_FILE))
        structureManager.setup(OFXUIEDITOR_DEFAULT_STRUCTURE_FILE);

    if(propertiesCollection.size() == 0 && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE))
        addPropertiesFile(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE);
}

void EditorBase::addPropertiesFile(const string& filePath){
    // remember which files we have loaded, for ::reload
    loadedPropertiesFiles.insert(filePath);

    // ofLogWarning() << "get rid of propertiesManager (2)";
    // dataRef->propertiesManager.load(filePath);
    propertiesCollection.loadJsonFromFile(filePath);
}

shared_ptr<NodeModel> EditorBase::get(const string& nodePath, bool recursive){
    auto nodeModelRef = make_shared<NodeModel>();
    nodeModelRef->set("id", nodePath);

    // try to find properties information
    {
        auto propertiesModelRef = propertiesCollection.findById(nodePath);
        if(propertiesModelRef){
            ofLogWarning() << "TODO: make he nodeModelRef actively 'follow' the propertiesModelRef";
            nodeModelRef->copy(propertiesModelRef);
        }
    }

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
