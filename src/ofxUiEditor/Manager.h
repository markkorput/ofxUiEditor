#pragma once

#include "EditorBase.h"

namespace ofxUiEditor {
    class Manager : public EditorBase<ofxInterface::Node> {



        // shared_ptr<NodeType> create(const string& nodePath, bool recursive=true);
        //
        // //! list lambda routines that can create appropriate node instances
        // std::map<string, INSTANTIATOR_FUNC> instantiator_funcs;
        // shared_ptr<NodeType> editorRootRef;
        // std::vector<shared_ptr<NodeType>> createdNodeRefs;
    };
}


// inline void addInstantiator(const string& id, INSTANTIATOR_FUNC func){
//     instantiator_funcs[id] = func;
// }
//
// inline void addType(const string& id, INSTANTIATOR_FUNC instantiator_func/*, ACTUATOR_FUNCTION actuator_func*/){
//     // addActuator(id, actuator_func);
//     addInstantiator(id, instantiator_func);
// }
