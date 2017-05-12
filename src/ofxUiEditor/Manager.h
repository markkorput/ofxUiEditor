#pragma once

#include "EditorBase.h"
#include "Instantiator.h"

namespace ofxUiEditor {
    class Manager : public EditorBase {

    public:
        void setup();

        // template<class NodeType>
        // shared_ptr<NodeType> instantiate(const string& nodePath, bool recursive=true);

    private:
        // Instantiator<ofxInterface::Node> instantiator;
    };
}
