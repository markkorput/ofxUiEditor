#pragma once

#include "EditorBase.h"

namespace ofxUiEditor {

    template<class NodeType>
    class Instantiator {

    public: // sub-types

        typedef std::function<shared_ptr<NodeType> ()> INSTANTIATOR_FUNC;

        typedef struct {
            shared_ptr<NodeType> nodeRef;
            shared_ptr<StructureInfo> infoRef;
        } NodeCreatedArgs;

    public:
        Instantiator();
        void setup(EditorBase* editorBase);

        shared_ptr<NodeType> instantiate(const string& nodePath, bool recursive=true);

    private:
        EditorBase* editorBase;
        shared_ptr<NodeType> rootRef;

        // //! list lambda routines that can create appropriate node instances
        // std::map<string, INSTANTIATOR_FUNC> instantiator_funcs;
        // shared_ptr<NodeType> editorRootRef;
        // std::vector<shared_ptr<NodeType>> createdNodeRefs;

    };
}

template<class NodeType>
ofxUiEditor::Instantiator<NodeType>::Instantiator(){
    rootRef = make_shared<NodeType>();
}

template<class NodeType>
void ofxUiEditor::Instantiator<NodeType>::setup(EditorBase* editorBase){
    this->editorBase = editorBase;

    // // NodeTypes supported by default (as they are part of the ofxInterface dependency)
    // addType(".SolidColorPanel",
    //     OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::SolidColorPanel)/*,
    //     PropertiesActuators::actuateSolidColorPanel*/);
    //
    // addType(".BitmapTextButton",
    //     OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::BitmapTextButton)/*,
    //     PropertiesActuators::actuateBitmapTextButton*/);
}

template<class NodeType>
shared_ptr<NodeType> ofxUiEditor::Instantiator<NodeType>::instantiate(const string& nodePath, bool recursive){
    // shared_ptr<NodeType> node;
    //
    // // try to find structure information
    // auto infoRef = structureManager.get(nodePath);
    // if(!infoRef)
    //     ofLogWarning() << "no structure infoRef found for nodePath: " << nodePath;
    //
    // // try to find instantiator func
    // auto iterator = instantiator_funcs.find(nodePath); // based on component id
    // if(iterator == instantiator_funcs.end() && infoRef) // based on classname
    //     iterator = instantiator_funcs.find("."+infoRef->getClass());
    //
    // // create our node instance; either through found instantiator lambda, or by generating default node type
    // if(iterator != instantiator_funcs.end()){
    //     node = (iterator->second)();
    // } else {
    //     // couldn't find custom node type, use default node type
    //     node = make_shared<NodeType>();
    // }
    //
    // if(infoRef){
    //     node->setName(infoRef->getName());
    //
    //     if(recursive){
    //         const vector<string>& childNames = infoRef->getChildNames();
    //         for(auto& childName : childNames){
    //             auto childNode = create(nodePath + StructureManager::SEPARATOR + childName, recursive);
    //             node->addChild(childNode.get());
    //         }
    //     }
    // }
    //
    // createdNodeRefs.push_back(node);
    // NodeCreatedArgs args;
    // args.nodeRef = node;
    // args.infoRef = infoRef;
    // nodeCreatedEvent.notifyListeners(args);
    // // editorRootRef->addChild(node.get());
    // return node;
}
