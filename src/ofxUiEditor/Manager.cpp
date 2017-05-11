#include "Manager.h"

using namespace ofxUiEditor;

// 
// // NodeTypes supported by default (as they are part of the ofxInterface dependency)
// addType(".SolidColorPanel",
//     OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::SolidColorPanel)/*,
//     PropertiesActuators::actuateSolidColorPanel*/);
//
// addType(".BitmapTextButton",
//     OFX_UI_EDITOR_INSTANTIATOR(ofxInterface::BitmapTextButton)/*,
//     PropertiesActuators::actuateBitmapTextButton*/);
//


// template<class NodeType>
// ofxUiEditor::EditorBase<NodeType>::EditorBase(){
//     editorRootRef = make_shared<NodeType>();
// }

// template<class NodeType>
// shared_ptr<NodeType> ofxUiEditor::EditorBase<NodeType>::create(const string& nodePath, bool recursive){
//     shared_ptr<NodeType> node;
//
//     // try to find structure information
//     auto infoRef = structureManager.get(nodePath);
//     if(!infoRef)
//         ofLogWarning() << "no structure infoRef found for nodePath: " << nodePath;
//
//     // try to find instantiator func
//     auto iterator = instantiator_funcs.find(nodePath); // based on component id
//     if(iterator == instantiator_funcs.end() && infoRef) // based on classname
//         iterator = instantiator_funcs.find("."+infoRef->getClass());
//
//     // create our node instance; either through found instantiator lambda, or by generating default node type
//     if(iterator != instantiator_funcs.end()){
//         node = (iterator->second)();
//     } else {
//         // couldn't find custom node type, use default node type
//         node = make_shared<NodeType>();
//     }
//
//     if(infoRef){
//         node->setName(infoRef->getName());
//
//         if(recursive){
//             const vector<string>& childNames = infoRef->getChildNames();
//             for(auto& childName : childNames){
//                 auto childNode = create(nodePath + StructureManager::SEPARATOR + childName, recursive);
//                 node->addChild(childNode.get());
//             }
//         }
//     }
//
//     createdNodeRefs.push_back(node);
//     NodeCreatedArgs args;
//     args.nodeRef = node;
//     args.infoRef = infoRef;
//     nodeCreatedEvent.notifyListeners(args);
//     // editorRootRef->addChild(node.get());
//     return node;
// }
