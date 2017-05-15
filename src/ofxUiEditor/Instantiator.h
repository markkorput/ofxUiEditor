#pragma once

#include "ofxLambdaEvent.h"

namespace ofxUiEditor {

    template<class BaseType>
    struct InstantiationArgs {
        shared_ptr<BaseType> instanceRef;
        shared_ptr<NodeModel> nodeModelRef;
    };

    template<class BaseType>
    class Instantiator {

    public: // sub-types
        typedef FUNCTION<shared_ptr<BaseType>(shared_ptr<NodeModel>)> InstantiatorFunc;

        typedef InstantiationArgs<BaseType> InstantiationArgs;

    public: // methods
        void setup();
        shared_ptr<BaseType> instantiate(shared_ptr<NodeModel> nodeModelRef, bool recursive=true);
        void addInstantiator(const string& identifier, InstantiatorFunc func);

    public: // events
        LambdaEvent<InstantiationArgs> instantiationEvent;

    private: // attributes
        std::map<string, InstantiatorFunc> instantiatorFuncs;
        // since ofxInterface currently doesn't use shared_ptr but raw pointers, we need to store the shared pointers somewhere,
        // otherwise the self-destruct. TODO: patch ofxInterface to work with shared_ptrs so we don't need thise redundant pointer management
        std::vector<shared_ptr<BaseType>> createdInstancesRefs;
    };
}

template<class BaseType>
void ofxUiEditor::Instantiator<BaseType>::setup(){
    addInstantiator(".Node", [](shared_ptr<ofxUiEditor::NodeModel> nodeModel){
        return make_shared<ofxInterface::Node>();
    });

    addInstantiator(".SolidColorPanel", [](shared_ptr<ofxUiEditor::NodeModel> nodeModel){
        return make_shared<ofxInterface::SolidColorPanel>();
    });

    addInstantiator(".BitmapTextButton", [](shared_ptr<ofxUiEditor::NodeModel> nodeModel){
        return make_shared<ofxInterface::BitmapTextButton>();
    });

    addInstantiator(".TextureButton", [](shared_ptr<ofxUiEditor::NodeModel> nodeModel){
        return make_shared<ofxInterface::TextureButton>();
    });
}

template<class BaseType>
shared_ptr<BaseType> ofxUiEditor::Instantiator<BaseType>::instantiate(shared_ptr<ofxUiEditor::NodeModel> nodeModelRef, bool recursive){
    shared_ptr<BaseType> instanceRef;

    // try to find custom instantiator based on id
    auto it = instantiatorFuncs.find(nodeModelRef->getId());
    // couldn't find id-based instantiator, try class-based
    if(it == instantiatorFuncs.end())
        it = instantiatorFuncs.find("."+nodeModelRef->getClass());

    if(it == instantiatorFuncs.end()){
        // use default instantiator
        instanceRef = make_shared<BaseType>();
    } else {
        // use found custom instantiator
        instanceRef = it->second(nodeModelRef);
    }

    instanceRef->setName(nodeModelRef->getName());
    // ofLogWarning() << "Using ofxInterface::Node's net setDeleteChildren(false) feature, TODO: perform cleanup maintenance ourselves!";
    instanceRef->setDeleteChildren(false);

    InstantiationArgs args;
    args.instanceRef = instanceRef;
    args.nodeModelRef = nodeModelRef;
    instantiationEvent.notifyListeners(args);

    if(recursive){
        for(auto childRef : nodeModelRef->getChildren()){
            instanceRef->addChild(instantiate(childRef, recursive).get());
        }
    }

    // we need to remember them, otherwise they might deallocate immediately...
    // TODO; create fork of ofxInterface that uses shared_ptr internally
    createdInstancesRefs.push_back(instanceRef);

    return instanceRef;
}

template<class BaseType>
void ofxUiEditor::Instantiator<BaseType>::addInstantiator(const string& identifier, InstantiatorFunc func){
    instantiatorFuncs[identifier] = func;
}
