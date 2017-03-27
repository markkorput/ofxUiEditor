#include "MeshDataManager.h"

namespace ofxUiEditor {
    
    template<class NodeType>
    class DataToNodeActuator {
      
        
    private: // callbacks
        
        void onDataChange(MeshData& data){
            performApply();
        }
        
    public:
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
        
        void apply(){
            if(nodeRef == nullptr || meshDataRef == nullptr){
                ofLogWarning() << "DataToNodeActuator can't apply when either node or mesh data is missing";
                return;
            }
            
            performApply();
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

    private:
        shared_ptr<MeshData> meshDataRef;
        shared_ptr<NodeType> nodeRef;
    };

    template<class NodeType>
    class NodeGenerator {
    public:
        void setup(MeshDataManager& meshDataManager){
            this->meshDataManager = &meshDataManager;
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
            auto node = make_shared<NodeType>();
            
            // the actuator monitors changes in the Meshdata instance
            // and applies them to the generated node in realtime
            auto actuator = make_shared<DataToNodeActuator<NodeType>>();
            actuator->setup(meshDataRef, node);
            actuator->apply();
            actuators.push_back(actuator);

            if(recursive){
                // get layout data for child elements
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

        
    private:
        MeshDataManager* meshDataManager;
        vector<shared_ptr<DataToNodeActuator<NodeType>>> actuators;
    };
}
