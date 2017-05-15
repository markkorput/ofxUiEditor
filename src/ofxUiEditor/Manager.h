#pragma once

#include "EditorBase.h"
#include "Instantiator.h"
#include "Actuator.h"
#include "Animator.h"

namespace ofxUiEditor {

    template<class NodeType>
    class Manager : public EditorBase {
    public:
        typedef FUNCTION<shared_ptr<NodeType>(shared_ptr<NodeModel>)> InstantiatorFunc;
        typedef FUNCTION<void(shared_ptr<NodeType>, const string&)> AttrActuatorFunc;

    public:
        void setup();
        void update(float dt);
        void reload();

        // instantiating
        shared_ptr<NodeType> instantiate(const string& nodePath, bool recursive=true);
        void addInstantiator(const string& identifier, InstantiatorFunc func);

        // actuating
        void addActuator(const string& identifier, const string& attribute_name, AttrActuatorFunc func){
            actuator.addActuator(identifier, attribute_name, func);
        }

        void addActuator(const string& identifier, const string& parent_identifier){
            actuator.addActuator(identifier, parent_identifier);
        }

        // animation
        shared_ptr<AnimationFloat> startAnimation(const string& identifier){
            return animator.startAnimation(identifier);
        }

    private:
        Instantiator<NodeType> instantiator;
        Actuator<NodeType> actuator;
        Animator animator;
    };
}

template<class NodeType>
void ofxUiEditor::Manager<NodeType>::setup(){
    EditorBase::setup();
    instantiator.setup();
    actuator.setup();
    animator.setup();

    // register listener that applies the properties in nodeModelRef to every view object that is being instantiated
    instantiator.instantiationEvent.addListener([this](InstantiationArgs<NodeType>& args){
        this->actuator.actuate(
            args.instanceRef, // view-object; target that all the properties need to be applied to
            args.nodeModelRef, // the nodeModel contains layout information
            true // active; this also updates our view object when property configurations change at runtime (for example because the proeprties.json get reloaded), mostly for debugging
        );
    }, this);
}

template<class NodeType>
void ofxUiEditor::Manager<NodeType>::update(float dt){
    animator.update(dt);
}

template<class NodeType>
void ofxUiEditor::Manager<NodeType>::reload(){
    EditorBase::reload();
    actuator.reload();
}

template<class NodeType>
shared_ptr<NodeType> ofxUiEditor::Manager<NodeType>::instantiate(const string& nodePath, bool recursive){
    // get layout-information for specified node
    auto nodeModelRef = this->get(nodePath, recursive);
    // give the information to the instantiator and return results
    return instantiator.instantiate(nodeModelRef, recursive);
}

template<class NodeType>
void ofxUiEditor::Manager<NodeType>::addInstantiator(const string& identifier, InstantiatorFunc func){
    instantiator.addInstantiator(identifier, func);
}
