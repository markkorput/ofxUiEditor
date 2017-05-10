#pragma once

#include "ofxCMS.h"
#include "EditorBase.h"

#define OFXUIEDITOR_DEFAULT_PROPERTIES_FILE "properties.json"

namespace ofxUiEditor {

    template<class NodeType>
    class EditorMain : public EditorBase<NodeType> {

    private: // sub-types
        EditorMain();

        class PropertiesModel : public ofxCMS::Model {
        public:
            void follow(shared_ptr<PropertiesModel> other){
                if(other == nullptr){
                    ofLogWarning() << "got nullptr";
                    return;
                }

                ofLogWarning() << "TODO: implement PropertiesModel.follow";
            }
        };

        typedef void (*ACTUATOR_FUNCTION)(shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<PropertiesModel> propertiesModelRef);

        class ActuatorModel : public ofxCMS::Model {
        public:
            ACTUATOR_FUNCTION func;
        };

        class ActuatorCollection : public ofxCMS::Collection<ActuatorModel>{

        };

        class NodeLink {
            public:
                void setup( shared_ptr<NodeType> _nodeRef,
                            shared_ptr<StructureInfo> _structRef,
                            shared_ptr<PropertiesModel> _propsRef,
                            shared_ptr<ActuatorCollection> actuatorCollectionRef){
                    // ofLog() << "NodeLink::setup";
                    nodeRef = _nodeRef;
                    structureRef = _structRef;
                    propertiesRef = _propsRef;
                    this->actuatorCollectionRef = actuatorCollectionRef;
                    ofLogVerbose() << "TODO: cache relevant actuators but also update on newly registered actuators";

                    ofLogWarning() << "TODO: implement realtime property change updates";
                    // propertiesRef->changeEvent.addListener([this](PropertiesItem &propsItem){
                    //     // ofLog() << "actuate callback";
                    //     this->actuateProperties();
                    // }, nodeRef.get());

                    actuateProperties();
                }

                void actuateProperties(){
                    ofLogVerbose() << "NodeLink::actuateProperties - updating node: " << nodeRef->getName();

                    if(!actuatorCollectionRef){
                        ofLogWarning() << "no actuator collection";
                        return;
                    }

                    auto actuatorModelRef = actuatorCollectionRef.findById(propertiesRef->getId());
                    if(actuatorModelRef)
                        actuatorModelRef->func(nodeRef, propertiesRef);

                    if(structureRef){
                        actuatorModelRef = actuatorCollectionRef.findById("."+structureRef->getClass());
                        if(actuatorModelRef)
                            actuatorModelRef->func(nodeRef, propertiesRef);
                    }
                }

                shared_ptr<NodeType> nodeRef;
                shared_ptr<StructureInfo> structureRef;
                shared_ptr<PropertiesModel> propertiesRef;
                shared_ptr<ActuatorCollection> actuatorCollectionRef;
        };

    public:
        void setup();

        void addPropertiesFile(const string& filePath);

    private:
        ofxCMS::Collection<PropertiesModel> propertiesCollection;
        std::set<string> loadedPropertiesFiles;
        std::vector<shared_ptr<NodeLink>> nodeLinkRefs;
        shared_ptr<ActuatorCollection> actuatorCollection;
    };
}

template<class NodeType>
ofxUiEditor::EditorMain<NodeType>::EditorMain(){
    actuatorCollection = make_shared<ActuatorCollection>();
}

template<class NodeType>
void ofxUiEditor::EditorMain<NodeType>::setup(){
    EditorBase<NodeType>::setup();

    if(propertiesCollection.size() == 0 && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE))
        addPropertiesFile(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE);

    nodeCreatedEvent.addListener([this](EditorBase::NodeCreatedArgs& args){
        auto infoRef = args.infoRef;

        auto propsModelRef = make_shared<PropertiesModel>();
        if(infoRef)
            propsModelRef->follow(*this->propertiesCollection.findById("."+infoRef->getClass())); // class-based properties
        propsModelRef->follow(*this->propertiesCollection.findById(infoRef->getId())); // component-id-based properties

        // create "link" used to update nodes when properties change at runtime
        auto nodeLinkRef = make_shared<NodeLink>();
        nodeLinkRef->setup(node, infoRef, propsModelRef, actuatorCollection);
        this->nodeLinkRefs.push_back(nodeLinkRef);
    }, this);
}

template<class NodeType>
void ofxUiEditor::addPropertiesFile(const string& filePath){
    // remember which files we have loaded, for ::reload
    loadedPropertiesFiles.insert(filePath);

    // ofLogWarning() << "get rid of propertiesManager (2)";
    // dataRef->propertiesManager.load(filePath);
    propertiesCollection.loadJsonFromFile(filePath);
}

template<class NodeType>
void Editor<NodeType>::addActuator(const string& id, ACTUATOR_FUNCTION func){
    auto actuatorRef = actuatorCollection.create();
    actuatorRef.set("id", id);
    actuatorRef.func = func;
}
