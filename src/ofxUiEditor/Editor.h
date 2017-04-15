// ofxAddons
#include "ofxInterface.h"
// local
#include "LambdaEvent.h"
#include "StructureManager.h"
#include "PropertiesManager.h"
#include "BasePropertiesActuator.h"

using namespace ofxInterface;

//
// Definition
//

namespace ofxUiEditor {

    template<class NodeType>
    class EditorSceneData {
    public:
        EditorSceneData() : sceneRef(nullptr){}
        EditorSceneData(shared_ptr<NodeType> scene) : sceneRef(scene){}

    public:
        shared_ptr<NodeType> sceneRef;
        std::vector<shared_ptr<LambdaEvent<TouchEvent>>> lambdaTouchEvents;
    };


    template<class NodeType>
    class Editor {

    public:
        Editor() : sceneData(nullptr),
                    current(nullptr),
                    structureManager(NULL),
                    propertiesManager(NULL){}
        ~Editor(){ destroy(); }

        void setup(shared_ptr<NodeType> newScene);
        void destroy(){ ofLogWarning() << "ofxUiEditor::Editor doesn't UNregister event listeners yet."; }

    public:
        shared_ptr<EditorSceneData<NodeType>> getSceneData() const { return sceneData; }
        // give the node that this editor instance points to
        NodeType* getCurrent() const { return current; }
        void setCurrent(NodeType* newCurrent){ current = newCurrent; }

        shared_ptr<Editor<NodeType>> node(const string& name) const;

        void use(StructureManager& structureManager);
        void use(PropertiesManager& propertiesManager);
        void addComponentPropertiesActuator(const string& componentId, shared_ptr<BasePropertiesActuator<NodeType>> actuatorRef);
        shared_ptr<NodeType> create(const string& nodePath, bool recursive=true);

    public: // register method for lambda register methods
        void onTouchDown(std::function<void (TouchEvent&)> func);

    protected:
        shared_ptr<Editor<NodeType>> clone() const;
        void clone(const Editor<NodeType> &original);
        shared_ptr<Editor<NodeType>> dummy() const;

    private:
        StructureManager* structureManager;
        PropertiesManager* propertiesManager;
        shared_ptr<EditorSceneData<NodeType>> sceneData;
        vector<shared_ptr<NodeType>> generatedNodes;
        NodeType* current;

        map<string, shared_ptr<BasePropertiesActuator<NodeType>>> componentPropertyActuators;
    };
}


//
// Implementation
//

using namespace ofxUiEditor;

template<class NodeType>
void Editor<NodeType>::setup(shared_ptr<NodeType> newScene){
    // create scene data instance
    sceneData = make_shared<EditorSceneData<NodeType>>();
    sceneData->sceneRef = newScene;
    // set our current node pointer to the given scene node (root)
    current = newScene.get();
}

template<class NodeType>
void Editor<NodeType>::use(StructureManager& structureManager){
    this->structureManager = &structureManager;
}

template<class NodeType>
void Editor<NodeType>::use(PropertiesManager& propertiesManager){
    this->propertiesManager = &propertiesManager;
}

template<class NodeType>
void Editor<NodeType>::addComponentPropertiesActuator(const string& componentId,
                        shared_ptr<BasePropertiesActuator<NodeType>> actuatorRef){
    actuatorRef->setComponentId(componentId);
    this->componentPropertyActuators[componentId] = actuatorRef;
}

template<class NodeType>
shared_ptr<NodeType> Editor<NodeType>::create(const string& nodePath, bool recursive){
    auto node = make_shared<NodeType>();
    // we need to cache our shared pointers, otherwise they'll auto-deallocate
    generatedNodes.push_back(node);

    // try to find structure information
    if(!structureManager){
        ofLogWarning() << "no StructureManager available to initialize nodePath: " << nodePath;
        return node;
    }

    auto infoRef = structureManager->get(nodePath);
    if(!infoRef){
        ofLogWarning() << "no structure data found for nodePath: " << nodePath;
        return node;
    }

    node->setName(infoRef->getName());

    // try to find and apply properties configuration
    if(propertiesManager){
        auto propsItemRef = propertiesManager->get(nodePath);
        if(propsItemRef){
            auto it = componentPropertyActuators.find(propsItemRef->getId());
            if(it != componentPropertyActuators.end()){
                ofLog() << "using custom actuator for: " << nodePath;
                it->second->actuate(node, propsItemRef);
            } else {
                ofLog() << "using DEFAULT actuator for: " << nodePath;
                // use default
                auto actuatorRef = make_shared<BasePropertiesActuator<NodeType>>();
                actuatorRef->actuate(node, propsItemRef);
            }
        }
    }

    if(recursive){
        const vector<string>& childNames = infoRef->getChildNames();
        for(auto& childName : childNames){
            auto childNode = create(nodePath + StructureManager::SEPARATOR + childName, recursive);
            node->addChild(childNode.get());
        }
    }

    return node;
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
