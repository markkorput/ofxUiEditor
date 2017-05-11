#pragma once

#include "ofxInterface.h"
#include "ofxCMS.h"
#include "ofxLambdaEvent/LambdaEvent.h"

#include "StructureManager.h"
#include "PropertiesActuators.h"
#include "NodeModel.h"
#include "macros.h"

#define OFXUIEDITOR_DEFAULT_STRUCTURE_FILE "structures.xml"
#define OFXUIEDITOR_DEFAULT_PROPERTIES_FILE "properties.json"

namespace ofxUiEditor {

    class PropertiesModel : public ofxCMS::Model {
    };

    template<class NodeType>
    class EditorBase {

    public:
        typedef std::function<shared_ptr<NodeType> ()> INSTANTIATOR_FUNC;

        typedef struct {
            shared_ptr<NodeType> nodeRef;
            shared_ptr<StructureInfo> infoRef;
        } NodeCreatedArgs;

    public:
        void setup();

        shared_ptr<NodeModel> get(const string& nodePath, bool recursive=true);
        void addPropertiesFile(const string& filePath);

    public:
        LambdaEvent<NodeCreatedArgs> nodeCreatedEvent;

    private:
        StructureManager structureManager;
        ofxCMS::Collection<PropertiesModel> propertiesCollection;
        std::set<string> loadedPropertiesFiles;
    };
}

template<class NodeType>
void ofxUiEditor::EditorBase<NodeType>::setup(){
    if(!structureManager.isLoaded() && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_STRUCTURE_FILE))
        structureManager.setup(OFXUIEDITOR_DEFAULT_STRUCTURE_FILE);

    if(propertiesCollection.size() == 0 && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE))
        addPropertiesFile(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE);
}

template<class NodeType>
void ofxUiEditor::EditorBase<NodeType>::addPropertiesFile(const string& filePath){
    // remember which files we have loaded, for ::reload
    loadedPropertiesFiles.insert(filePath);

    // ofLogWarning() << "get rid of propertiesManager (2)";
    // dataRef->propertiesManager.load(filePath);
    propertiesCollection.loadJsonFromFile(filePath);
}

template<class NodeType>
shared_ptr<ofxUiEditor::NodeModel> ofxUiEditor::EditorBase<NodeType>::get(const string& nodePath, bool recursive){
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

        if(recursive){
            const vector<string>& childNames = infoRef->getChildNames();
            for(auto& childName : childNames){
                nodeModelRef->addChild(get(nodePath + StructureManager::SEPARATOR + childName, recursive));
            }
        }
    }
    return nodeModelRef;
}
