#pragma once

#include "ofxCMS.h"

namespace ofxUiEditor {
    class NodeModel : public ofxCMS::Model {
    public:
        NodeModel() : parentRef(nullptr){}

        void setup(shared_ptr<NodeModel> parent, std::vector<shared_ptr<NodeModel>> children){
            this->parentRef = parent;
            for(auto kid : children){
                this->childrenRefs.push_back(kid);
            }
        }

        void addChild(shared_ptr<NodeModel> child){
            childrenRefs.push_back(child);
        }

        shared_ptr<NodeModel> getParent() const { return parentRef; }
        const std::vector<shared_ptr<NodeModel>> &getChildren() const { return childrenRefs; }

        size_t size(bool recursive=false){
            size_t count = childrenRefs.size();
            if(recursive){
                for(auto childRef : childrenRefs){
                    count += childRef->size(recursive);
                }
            }

            return count;
        }

    private:
        shared_ptr<NodeModel> parentRef;
        std::vector<shared_ptr<NodeModel>> childrenRefs;
    };
}
