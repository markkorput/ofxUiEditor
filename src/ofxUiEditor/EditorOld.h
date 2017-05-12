#include "ofxInterface.h"
#include "ofxCMS.h"
// local
#include "ofxLambdaEvent/LambdaEvent.h"
#include "StructureManager.h"
#include "PropertiesManager.h"
#include "PropertiesActuators.h"
#include "macros.h"

using namespace ofxInterface;

#define DEFAULT_STRUCTURE_FILE "structures.xml"
#define DEFAULT_PROPERTIES_FILE "properties.json"

//
// Definition
//

namespace ofxUiEditor {

    template<class NodeType>
    class EditorOld {

        typedef std::function<shared_ptr<NodeType> ()> INSTANTIATOR_FUNC;
        typedef void (*ACTUATOR_FUNCTION)(shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef);

    private: // sub-types

        typedef struct {
            string id;
            ACTUATOR_FUNCTION func;
        } ComponentActuator;

        class NodeLink {
            public:
                void setup( shared_ptr<NodeType> _nodeRef,
                            shared_ptr<StructureInfo> _structRef,
                            shared_ptr<PropertiesItem> _propsRef,
                            std::vector<shared_ptr<ComponentActuator>> &actuators){
                    // ofLog() << "NodeLink::setup";
                    nodeRef = _nodeRef;
                    structureRef = _structRef;
                    propertiesRef = _propsRef;
                    actuatorRefs = &actuators;
                    ofLogVerbose() << "TODO: cache relevant actuators but also update on newly registered actuators";

                    propertiesRef->changeEvent.addListener([this](PropertiesItem &propsItem){
                        // ofLog() << "actuate callback";
                        this->actuateProperties();
                    }, nodeRef.get());

                    actuateProperties();
                }

                void actuateProperties(){
                    ofLogVerbose() << "NodeLink::actuateProperties - updating node: " << nodeRef->getName();
                    bool bCustom = false;

                    for(auto actuatorRef : (*actuatorRefs)){
                        if(actuatorRef->id == propertiesRef->getId() || (structureRef && (actuatorRef->id == ("."+structureRef->getClass())))){
                            bCustom = true;
                            // apply custom actuator
                            (*actuatorRef->func)(nodeRef, propertiesRef);
                        }
                    }

                    if(!bCustom)
                        PropertiesActuators::actuateNode(nodeRef, propertiesRef);
                }

                shared_ptr<NodeType> nodeRef;
                shared_ptr<StructureInfo> structureRef;
                shared_ptr<PropertiesItem> propertiesRef;
                std::vector<shared_ptr<ComponentActuator>>* actuatorRefs;
        };

        class Data {
            public:
                Data() : sceneRef(nullptr){
                }
                Data(shared_ptr<NodeType> scene) : sceneRef(scene){
                }

            public:
                shared_ptr<NodeType> sceneRef;
                shared_ptr<NodeType> editorRootRef;
                std::vector<shared_ptr<LambdaEvent<TouchEvent>>> lambdaTouchEvents;
                std::vector<shared_ptr<NodeLink>> nodeLinkRefs;

                StructureManager* structureManager;
                StructureManager privateStructureManager;
                PropertiesManager propertiesManager;

                std::map<string, INSTANTIATOR_FUNC> instantiator_funcs;
                std::vector<shared_ptr<ComponentActuator>> actuatorsRefs;

                std::set<string> loadedPropertiesFiles;
                // ofxCMS::Collection<ofxCMS::Model> propertiesCollection;
        };

    public: // common methods

        EditorOld() :  dataRef(nullptr),
                    current(nullptr){
            // create scene dataRef instance
            dataRef = make_shared<Data>();
            dataRef->editorRootRef = make_shared<NodeType>();
            current = dataRef->editorRootRef.get();
        }
        ~EditorOld(){ destroy(); }

        void setup();
        void setup(shared_ptr<NodeType> newScene);
        void update(float dt);
        void destroy(){ ofLogWarning() << "ofxUiEditor::EditorOld doesn't UNregister event listeners yet."; }

    public: // operations

        shared_ptr<NodeType> create(const string& nodePath, bool recursive=true);
        void remove(shared_ptr<NodeType> node);
        void reload();

    public: // getters / adders

        inline shared_ptr<EditorOld<NodeType>> operator[](const string& name) const { return this->node(name); }
        shared_ptr<EditorOld<NodeType>> node(const string& name) const;
        void animate(const string& id);
        shared_ptr<Data> getData() const { return dataRef; }
        // give the node that this editor instance points to
        NodeType* getCurrent() const { return current; }
        void setCurrent(NodeType* newCurrent){ current = newCurrent; }

        void addStructureFile(const string& filePath){
            if(dataRef->structureManager != NULL)
                ofLogWarning() << "For now only one structures file at-a-time supported";

            dataRef->privateStructureManager.setup(filePath);
            use(dataRef->privateStructureManager);
        }

        void addPropertiesFile(const string& filePath){
            dataRef->loadedPropertiesFiles.insert(filePath); // remember which files we have loaded, for ::reload
            ofLogWarning() << "get rid of propertiesManager (2)";
            dataRef->propertiesManager.load(filePath);
            // dataRef->propertiesCollection.loadJsonFromFile(filePath);
            // dataRef->propertiesCollection.create();
        }

        void use(StructureManager& structureManager);
        void addActuator(const string& id, ACTUATOR_FUNCTION func);
        inline void addInstantiator(const string& id, INSTANTIATOR_FUNC func){
            dataRef->instantiator_funcs[id] = func;
        }
        inline void addType(const string& id, INSTANTIATOR_FUNC instantiator_func, ACTUATOR_FUNCTION actuator_func){
            addActuator(id, actuator_func);
            addInstantiator(id, instantiator_func);
        }

    public: // register method for lambda register methods

        void onTouchDown(std::function<void (TouchEvent&)> func);

    protected: // cloning methods

        shared_ptr<EditorOld<NodeType>> clone() const;
        void clone(const EditorOld<NodeType> &original);
        shared_ptr<EditorOld<NodeType>> dummy() const;

    public: // attributes

        shared_ptr<Data> dataRef;
        NodeType* current;

    };
}


//
// Implementation
//

using namespace ofxUiEditor;

template<class NodeType>
void EditorOld<NodeType>::setup(){
    // load default file
    if(!dataRef->structureManager && ofFile::doesFileExist(DEFAULT_STRUCTURE_FILE))
        addStructureFile(DEFAULT_STRUCTURE_FILE);

    // load default file
    if(ofFile::doesFileExist(DEFAULT_PROPERTIES_FILE))
        addPropertiesFile(DEFAULT_PROPERTIES_FILE);

    addType(".SolidColorPanel",
        OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::SolidColorPanel),
        PropertiesActuators::actuateSolidColorPanel);

    addType(".BitmapTextButton",
        OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::BitmapTextButton),
        PropertiesActuators::actuateBitmapTextButton);
}

template<class NodeType>
void EditorOld<NodeType>::setup(shared_ptr<NodeType> newScene){
    setup();
    dataRef->sceneRef = newScene;
    // set our current node pointer to the given scene node (root)
    current = newScene.get();
}

template<class NodeType>
void EditorOld<NodeType>::update(float dt){
    ofLogWarning() << "update all active animations";
}

template<class NodeType>
void EditorOld<NodeType>::use(StructureManager& structureManager){
    dataRef->structureManager = &structureManager;
}

template<class NodeType>
void EditorOld<NodeType>::addActuator(const string& id, ACTUATOR_FUNCTION func){
    auto actuator = make_shared<ComponentActuator>();
    actuator->id = id;
    actuator->func = func;
    dataRef->actuatorsRefs.push_back(actuator);
}

template<class NodeType>
shared_ptr<NodeType> EditorOld<NodeType>::create(const string& nodePath, bool recursive){
    shared_ptr<NodeType> node;
    shared_ptr<StructureInfo> infoRef;

    // try to find structure information
    if(!dataRef->structureManager){
        ofLogWarning() << "no StructureManager available to initialize nodePath: " << nodePath;
        infoRef = nullptr;
    } else {
        infoRef = dataRef->structureManager->get(nodePath);

        if(!infoRef)
            ofLogWarning() << "no structure dataRef found for nodePath: " << nodePath;
    }

    // create our node instance
    auto iterator = dataRef->instantiator_funcs.find(nodePath); // based on component id
    if(iterator == dataRef->instantiator_funcs.end() && infoRef) // based on classname
        iterator = dataRef->instantiator_funcs.find("."+infoRef->getClass());

    if(iterator != dataRef->instantiator_funcs.end()){
        node = (iterator->second)();
    } else {
        node = make_shared<NodeType>();
    }

    if(infoRef)
        node->setName(infoRef->getName());

    {   // try to find and apply properties configurations
        auto propsItemRef = make_shared<PropertiesItem>();
        if(infoRef)
            propsItemRef->follow(*dataRef->propertiesManager.get("."+infoRef->getClass())); // class-based properties
        propsItemRef->follow(*dataRef->propertiesManager.get(infoRef->getId())); // component-id-based properties

        // create "link" used to update nodes when properties change at runtime
        auto nodeLinkRef = make_shared<NodeLink>();
        nodeLinkRef->setup(node, infoRef, propsItemRef, dataRef->actuatorsRefs);

        if(dataRef){
            dataRef->nodeLinkRefs.push_back(nodeLinkRef);
            // ofLog() << "link added";
        } else {
            ofLogWarning() << "Editor not setup-ed; no dataRef instance";
        }
    }

    if(recursive && infoRef){
        const vector<string>& childNames = infoRef->getChildNames();
        for(auto& childName : childNames){
            auto childNode = create(nodePath + StructureManager::SEPARATOR + childName, recursive);
            node->addChild(childNode.get());
        }
    }

    dataRef->editorRootRef->addChild(node.get());
    return node;
}

template<class NodeType>
void EditorOld<NodeType>::remove(shared_ptr<NodeType> node){
    ofLog() << "Removing node: " << node->getName();

    if(!dataRef){
        ofLogWarning() << "no dataRef instance; setup should be called first";
        return;
    }

    for(auto it=dataRef->nodeLinkRefs.begin(); it != dataRef->nodeLinkRefs.end(); it++){
        if((*it)->nodeRef == node){
            dataRef->nodeLinkRefs.erase(it);
            return;
        }
    }
}

template<class NodeType>
void EditorOld<NodeType>::reload(){
    for(auto& filePath : dataRef->loadedPropertiesFiles){
        ofLogWarning() << "get rid of propertiesManager (1)";
        dataRef->propertiesManager.load(filePath);
        // dataRef->propertiesCollection.loadJsonFromFile(filePath);

    }
}

template<class NodeType>
void EditorOld<NodeType>::onTouchDown(std::function<void (TouchEvent&)> func){
    // we'll need a node to listen to
    if(!current){
        ofLogWarning() << "onTouchDown without a current node";
        return;
    }

    // create lambdaEvent instance and store it in our scene's list
    auto lambdaE = make_shared<LambdaEvent<TouchEvent>>();

    dataRef->lambdaTouchEvents.push_back(lambdaE);
    // make our new lambdaEvent listener for, and forward, our node's touchDown event
    lambdaE->forward(current->eventTouchDown);
    // finally register the given listener as listener for our lambda event
    lambdaE->addListener(func, (void*)dataRef.get() /* use scene dataRef as "owner" of the callback */);
}

// returns new cloned instance, pointing at the specified node
template<class NodeType>
shared_ptr<EditorOld<NodeType>> EditorOld<NodeType>::node(const string& name) const {
    if(!current)
        return dummy();

    // TODO: support for more specific component path
    NodeType* n = current->getChildWithName(name);

    if(!n)
        ofLogWarning() << "node(\""+name+"\"); doesn't exist.";

    auto result = clone();
    result->setCurrent(n);

    return result;
}

template<class NodeType>
void EditorOld<NodeType>::animate(const string& id){
    ofLogWarning() << "TODO: initiate animation";
}

// returns a new instance that is a clone of the current instance
template<class NodeType>
shared_ptr<EditorOld<NodeType>> EditorOld<NodeType>::clone() const {
    auto c = make_shared<EditorOld<NodeType>>();
    c->clone(*this);
    return c;
}

// make the current instance a clone of the given instance
template<class NodeType>
void EditorOld<NodeType>::clone(const EditorOld<NodeType> &original){
    dataRef = original.getData();
    current = original.getCurrent();
}

template<class NodeType>
shared_ptr<EditorOld<NodeType>> EditorOld<NodeType>::dummy() const {
    auto c = make_shared<EditorOld<NodeType>>();
    return c;
}
