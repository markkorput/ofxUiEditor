#pragma once

#include "EditorBase.h"
#include "Instantiator.h"

namespace ofxUiEditor {
    template<class NodeType>
    class Manager : public EditorBase {

    public:
        shared_ptr<NodeType> instantiate(const string& nodePath, bool recursive=true);

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
