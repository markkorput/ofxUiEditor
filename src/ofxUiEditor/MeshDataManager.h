#pragma once

#include "MeshData.h"

#ifdef OFX_UI_EDITOR_OSC
#include "ofxOscMessage.h"
#endif

namespace ofxUiEditor {

    class MeshDataManager {

    public: // common methods

        void draw();
        void drawItem(shared_ptr<MeshData> item);

        bool saveToFile(const string& filePath);
        bool loadFromFile(const string& filePath);

    public: // getters/setters
        shared_ptr<MeshData> find(const string &id);
        shared_ptr<MeshData> get(const string &id);

        vector<shared_ptr<MeshData>> getRootItems();
        vector<shared_ptr<MeshData>> getChildren(const string &parentId);

#ifdef OFX_UI_EDITOR_OSC
    public: // helper methods
        bool processOscMessage(ofxOscMessage msg);
#endif

    template<class NodeType>
    NodeType* generateNode(const string& layoutId, bool recursive=true);
        
    template<class NodeType>
    NodeType* generateNode(shared_ptr<MeshData> meshDataRef, bool recursive=true);

    private: // callbacks
        void onItemChange(MeshData &item);

    public: // events
        ofEvent<MeshData> newItemEvent, itemUpdatedEvent;

    private: // attributes
        map<string, shared_ptr<MeshData>> items;
    };
};


//
// template method(s) implementation
//

using namespace ofxUiEditor;

template<class NodeType>
NodeType* MeshDataManager::generateNode(const string& layoutId, bool recursive){
    auto meshDataRef = find(layoutId);

    if(meshDataRef == nullptr){
        ofLogWarning() << "Can't find layout data for id: " << layoutId << ". Can't generate node.";
        return nullptr;
    }

    return generateNode<NodeType>(meshDataRef, recursive);
}

template<class NodeType>
NodeType* MeshDataManager::generateNode(shared_ptr<MeshData> meshDataRef, bool recursive){
    // generate node and apply orientation, scale, position and size
    auto node = new NodeType();
    node->setOrientation(meshDataRef->getRotation());
    node->setScale(meshDataRef->getScale());
    node->setPosition(meshDataRef->getPosition());
    node->setSize(meshDataRef->getVertBoundsSize());

    if(recursive){
        // get layout data for child elements
        vector<shared_ptr<MeshData>> childMeshDataRefs = getChildren(meshDataRef->getId());
        
        for(auto childMeshDataRef : childMeshDataRefs){
            // recursively call this method to generate child elements
            auto childNode = generateNode<NodeType>(childMeshDataRef, recursive);

            if(childNode)
                node->addChild(childNode);
        }
    }

    return node;
}
