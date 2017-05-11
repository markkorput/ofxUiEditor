#pragma once

#include "MeshDataManager.h"

namespace ofxUiEditor {

    template<class NodeType>
    class DataToNodeActuator {

    private: // callbacks

        void onDataChange(MeshData& data){
            performApply();
            ofNotifyEvent(appliedEvent, *this);
        }

    public: // commond methods

        DataToNodeActuator() : meshDataRef(nullptr), nodeRef(nullptr){
        }

        ~DataToNodeActuator(){
            destroy();
        }

        void setup(shared_ptr<MeshData> meshData, shared_ptr<NodeType> node){
            meshDataRef = meshData;
            nodeRef = node;
            ofAddListener(meshDataRef->changeEvent, this, &DataToNodeActuator<NodeType>::onDataChange);
        }

        void destroy(){
            if(meshDataRef)
                ofRemoveListener(meshDataRef->changeEvent, this, &DataToNodeActuator<NodeType>::onDataChange);

            nodeRef = nullptr;
            meshDataRef = nullptr;
        }

        void apply(bool notify=true){
            if(nodeRef == nullptr || meshDataRef == nullptr){
                ofLogWarning() << "DataToNodeActuator can't apply when either node or mesh data is missing";
                return;
            }

            performApply();

            if(notify)
                ofNotifyEvent(appliedEvent, *this);
        }

        shared_ptr<NodeType> getNode() const {
            return nodeRef;
        }

    private:

        void performApply(){
            // apply orientation, scale, position and size
            nodeRef->setName(meshDataRef->getName());
            nodeRef->setOrientation(meshDataRef->getRotation());
            nodeRef->setScale(meshDataRef->getScale());
            nodeRef->setPosition(meshDataRef->getPosition());
            nodeRef->setSize(meshDataRef->getVertBoundsSize());
        }

    public: // events

        ofEvent<DataToNodeActuator<NodeType>> appliedEvent;

    private:
        shared_ptr<MeshData> meshDataRef;
        shared_ptr<NodeType> nodeRef;
    };


    template<class NodeType>
    class NodeGenerator {
    public:

        NodeGenerator(){
            params.setName("NodeGenerator");
            params.add(realtimeUpdatesParam.set("realtime-updates", true));
            instantiatorMethod = nullptr;
        }

        ~NodeGenerator(){
            destroy();
        }

        void setup(MeshDataManager& meshDataManager){
            this->meshDataManager = &meshDataManager;
        }

        void destroy(){
            for(auto actuator : actuators){
                ofRemoveListener(actuator->appliedEvent, this, &NodeGenerator<NodeType>::onActuatorApplied);
            }
        }

        shared_ptr<NodeType> generateNode(const string& layoutId, bool recursive=true){
            auto meshDataRef = meshDataManager->find(layoutId);

            if(meshDataRef == nullptr){
                ofLogWarning() << "Can't find layout data for id: " << layoutId << ". Can't generate node.";
                return nullptr;
            }

            return generateNode(meshDataRef, recursive);
        }

        shared_ptr<NodeType> generateNode(shared_ptr<MeshData> meshDataRef, bool recursive=true){
            // generate node
            shared_ptr<NodeType> node;

            if(instantiatorMethod)
                node = instantiatorMethod(meshDataRef);
            else
                node = make_shared<NodeType>();

            generatedNodes.push_back(node);

            // the actuator monitors changes in the Meshdata instance
            // and applies them to the generated node in realtime
            {
                auto actuator = make_shared<DataToNodeActuator<NodeType>>();
                actuator->setup(meshDataRef, node);
                // the actuator knows how to "apply" meshData to the node
                actuator->apply(false);

                // if we don't store the actuator in our actuators list
                // the shared ptr will go out of scope and de-allocate,
                // destroying the realtime update link inside the actuator
                if(realtimeUpdatesParam.get()){
                    // register listener to forward events to our nodeUpdatedEvent
                    ofAddListener(actuator->appliedEvent, this, &NodeGenerator<NodeType>::onActuatorApplied);
                    actuators.push_back(actuator);
                }
            }

            // also generate all child elements?
            if(recursive){
                // get data
                vector<shared_ptr<MeshData>> childMeshDataRefs = meshDataManager->getChildren(meshDataRef->getId());

                for(auto childMeshDataRef : childMeshDataRefs){
                    // recursively call this method to generate child elements
                    auto childNode = generateNode(childMeshDataRef, recursive);

                    if(childNode)
                        node->addChild(childNode.get());
                }
            }

            return node;
        }

        void setInstantiator(std::function<shared_ptr<NodeType> (shared_ptr<MeshData>)> func){
            instantiatorMethod = func;
        }

    private: // callbacks

        void onActuatorApplied(DataToNodeActuator<NodeType>& actuator){
            ofNotifyEvent(nodeUpdatedEvent, *actuator.getNode().get());
        }

    public: // events
        ofEvent<NodeType> nodeUpdatedEvent;

    public: // params
        ofParameterGroup params;
        ofParameter<bool> realtimeUpdatesParam;

    private: // attributes
        MeshDataManager* meshDataManager;
        vector<shared_ptr<DataToNodeActuator<NodeType>>> actuators;
        vector<shared_ptr<NodeType>> generatedNodes;
        std::function<shared_ptr<NodeType> (shared_ptr<MeshData>)> instantiatorMethod;
    };
}
