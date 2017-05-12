#pragma once

#include "EditorBase.h"
#include "Instantiator.h"
#include "Actuator.h"

namespace ofxUiEditor {
    template<class NodeType>
    class Manager : public EditorBase {
    public:
        typedef FUNCTION<shared_ptr<NodeType>(shared_ptr<NodeModel>)> InstantiatorFunc;

    public:
        // instantiating
        shared_ptr<NodeType> instantiate(const string& nodePath, bool recursive=true);
        void addInstantiator(const string& identifier, InstantiatorFunc func);


    private:
        Instantiator<NodeType> instantiator;
    };
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
