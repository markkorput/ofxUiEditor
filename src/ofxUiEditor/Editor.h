// ofxAddons
#include "ofxInterface.h"
// local
#include "LambdaEvent.h"
#include "StructureManager.h"
#include "PropertiesManager.h"
#include "PropertiesActuators.h"

using namespace ofxInterface;

#define DEFAULT_STRUCTURE_FILE "structures.xml"
#define DEFAULT_PROPERTIES_FILE "properties.json"

//
// Definition
//

namespace ofxUiEditor {

    template<class NodeType>
    class Editor {

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
                            std::vector<shared_ptr<ComponentActuator>> &componentPropertiesActuators){
                    // ofLog() << "NodeLink::setup";
                    nodeRef = _nodeRef;
                    structureRef = _structRef;
                    propertiesRef = _propsRef;
                    actuatorRefs = &componentPropertiesActuators;
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

        class EditorSceneData {
            public:
                EditorSceneData() : sceneRef(nullptr){}
                EditorSceneData(shared_ptr<NodeType> scene) : sceneRef(scene){}

            public:
                shared_ptr<NodeType> sceneRef;
                std::vector<shared_ptr<LambdaEvent<TouchEvent>>> lambdaTouchEvents;
                std::vector<shared_ptr<NodeLink>> nodeLinkRefs;
        };

    public: // common methods
        Editor() :  sceneData(nullptr),
                    current(nullptr),
                    structureManager(NULL){}
        ~Editor(){ destroy(); }

        void setup();
        void setup(shared_ptr<NodeType> newScene);
        void destroy(){ ofLogWarning() << "ofxUiEditor::Editor doesn't UNregister event listeners yet."; }

    public:

        shared_ptr<NodeType> create(const string& nodePath, bool recursive=true);
        void remove(shared_ptr<NodeType> node);
        void reload();

        shared_ptr<Editor<NodeType>> node(const string& name) const;
        shared_ptr<EditorSceneData> getSceneData() const { return sceneData; }
        // give the node that this editor instance points to
        NodeType* getCurrent() const { return current; }
        void setCurrent(NodeType* newCurrent){ current = newCurrent; }

        void addStructureFile(const string& filePath){
            if(structureManager != NULL)
                ofLogWarning() << "For now only one structures file at-a-time supported";

            privateStructureManager.setup(filePath);
            use(privateStructureManager);
        }

        void addPropertiesFile(const string& filePath){
            loadedPropertiesFiles.insert(filePath); // remember which files we have loaded, for ::reload
            propertiesManager.load(filePath);
        }

        void use(StructureManager& structureManager);
        void addActuator(const string& id, ACTUATOR_FUNCTION func);
        void addInstantiator(const string& id, INSTANTIATOR_FUNC func){
            instantiator_funcs[id] = func;
        }

    public: // register method for lambda register methods
        void onTouchDown(std::function<void (TouchEvent&)> func);

    protected:

        shared_ptr<Editor<NodeType>> clone() const;
        void clone(const Editor<NodeType> &original);
        shared_ptr<Editor<NodeType>> dummy() const;

    private:
        StructureManager* structureManager;
        StructureManager privateStructureManager;
        PropertiesManager propertiesManager;
        shared_ptr<EditorSceneData> sceneData;

        NodeType* current;

        std::map<string, INSTANTIATOR_FUNC> instantiator_funcs;
        std::vector<shared_ptr<ComponentActuator>> componentPropertiesActuators;

        std::set<string> loadedPropertiesFiles;
    };
}


//
// Implementation
//

using namespace ofxUiEditor;

template<class NodeType>
void Editor<NodeType>::setup(){
    // load default file
    if(!structureManager && ofFile::doesFileExist(DEFAULT_STRUCTURE_FILE))
        addStructureFile(DEFAULT_STRUCTURE_FILE);

    // load default file
    if(ofFile::doesFileExist(DEFAULT_PROPERTIES_FILE))
        addPropertiesFile(DEFAULT_PROPERTIES_FILE);

    // create scene data instance
    sceneData = make_shared<EditorSceneData>();
}

template<class NodeType>
void Editor<NodeType>::setup(shared_ptr<NodeType> newScene){
    setup();
    sceneData->sceneRef = newScene;
    // set our current node pointer to the given scene node (root)
    current = newScene.get();
}

template<class NodeType>
void Editor<NodeType>::use(StructureManager& structureManager){
    this->structureManager = &structureManager;
}

template<class NodeType>
void Editor<NodeType>::addActuator(const string& id, ACTUATOR_FUNCTION func){
    auto actuator = make_shared<ComponentActuator>();
    actuator->id = id;
    actuator->func = func;
    componentPropertiesActuators.push_back(actuator);
}

template<class NodeType>
shared_ptr<NodeType> Editor<NodeType>::create(const string& nodePath, bool recursive){
    shared_ptr<NodeType> node;
    shared_ptr<StructureInfo> infoRef;

    // try to find structure information
    if(!structureManager){
        ofLogWarning() << "no StructureManager available to initialize nodePath: " << nodePath;
        infoRef = nullptr;
    } else {
        infoRef = structureManager->get(nodePath);

        if(!infoRef)
            ofLogWarning() << "no structure data found for nodePath: " << nodePath;
    }

    // create our node instance
    auto iterator = instantiator_funcs.find(nodePath); // based on component id
    if(iterator == instantiator_funcs.end() && infoRef) // based on classname
        iterator = instantiator_funcs.find("."+infoRef->getClass());

    if(iterator != instantiator_funcs.end()){
        node = (iterator->second)();
    } else {
        node = make_shared<NodeType>();
    }

    if(infoRef)
        node->setName(infoRef->getName());

    {   // try to find and apply properties configurations
        auto propsItemRef = make_shared<PropertiesItem>();
        if(infoRef)
            propsItemRef->follow(*propertiesManager.get("."+infoRef->getClass())); // class-based properties
        propsItemRef->follow(*propertiesManager.get(infoRef->getId())); // component-id-based properties

        // create "link" used to update nodes when properties change at runtime
        auto nodeLinkRef = make_shared<NodeLink>();
        nodeLinkRef->setup(node, infoRef, propsItemRef, componentPropertiesActuators);

        if(sceneData){
            sceneData->nodeLinkRefs.push_back(nodeLinkRef);
            // ofLog() << "link added";
        } else {
            ofLogWarning() << "Editor not setup-ed; no sceneData instance";
        }
    }

    if(recursive && infoRef){
        const vector<string>& childNames = infoRef->getChildNames();
        for(auto& childName : childNames){
            auto childNode = create(nodePath + StructureManager::SEPARATOR + childName, recursive);
            node->addChild(childNode.get());
        }
    }

    return node;
}

template<class NodeType>
void Editor<NodeType>::remove(shared_ptr<NodeType> node){
    ofLog() << "Removing node: " << node->getName();

    if(!sceneData){
        ofLogWarning() << "no sceneData instance; setup should be called first";
        return;
    }

    for(auto it=sceneData->nodeLinkRefs.begin(); it != sceneData->nodeLinkRefs.end(); it++){
        if((*it)->nodeRef == node){
            sceneData->nodeLinkRefs.erase(it);
            return;
        }
    }
}

template<class NodeType>
void Editor<NodeType>::reload(){
    for(auto& filePath : loadedPropertiesFiles){
        propertiesManager.load(filePath);
    }
}

template<class NodeType>
void Editor<NodeType>::onTouchDown(std::function<void (TouchEvent&)> func){
    // we'll need a node to listen to
    if(!current){
        ofLogWarning() << "onTouchDown without a current node";
        return;
    }

    // create lambdaEvent instance and store it in our scene's list
    auto lambdaE = make_shared<LambdaEvent<TouchEvent>>();

    sceneData->lambdaTouchEvents.push_back(lambdaE);
    // make our new lambdaEvent listener for, and forward, our node's touchDown event
    lambdaE->forward(current->eventTouchDown);
    // finally register the given listener as listener for our lambda event
    lambdaE->addListener(func, (void*)sceneData.get() /* use scene data as "owner" of the callback */);
}

// returns new cloned instance, pointing at the specified node
template<class NodeType>
shared_ptr<Editor<NodeType>> Editor<NodeType>::node(const string& name) const {
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

// returns a new instance that is a clone of the current instance
template<class NodeType>
shared_ptr<Editor<NodeType>> Editor<NodeType>::clone() const {
    auto c = make_shared<Editor<NodeType>>();
    c->clone(*this);
    return c;
}

// make the current instance a clone of the given instance
template<class NodeType>
void Editor<NodeType>::clone(const Editor<NodeType> &original){
    sceneData = original.getSceneData();
    current = original.getCurrent();
}

template<class NodeType>
shared_ptr<Editor<NodeType>> Editor<NodeType>::dummy() const {
    auto c = make_shared<Editor<NodeType>>();
    return c;
}
