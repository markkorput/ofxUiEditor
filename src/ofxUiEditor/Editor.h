#include "ofxInterface.h"

using namespace ofxInterface;

//
// Definition
//

namespace ofxUiEditor {

    template<class NodeType>
    class EditorSceneData {
    public:
        EditorSceneData() : sceneRef(nullptr){}
        EditorSceneData(shared_ptr<NodeType> scene) : sceneRef(scene){}

    public:
        shared_ptr<NodeType> sceneRef;
    };


    template<class NodeType>
    class Editor {

    public:
        Editor() : sceneData(nullptr), current(nullptr){}
        void setup(shared_ptr<NodeType> newScene);

    public:
        EditorSceneData<NodeType> getSceneData(){ return sceneData; }
        // give the node that this editor instance points to
        shared_ptr<NodeType> getNode(){ return current; }
        shared_ptr<NodeType> getCurrent(){ return current; }
        void setCurrent(shared_ptr<NodeType> newCurrent){ current = newCurrent; }

    protected:
        shared_ptr<Editor<NodeType>> clone();
        void clone(const Editor<NodeType> &original);

        shared_ptr<Editor<NodeType>> node(const string& name);

    private:
        shared_ptr<EditorSceneData<NodeType>> sceneData;
        shared_ptr<NodeType> current;

    };
}


//
// Implementation
//

using namespace ofxUiEditor;

template<class NodeType>
void Editor<NodeType>::setup(shared_ptr<NodeType> newScene){
    // create scene data instance
    sceneData = make_shared<EditorSceneData<NodeType>>();
    sceneData->sceneRef = newScene;
    // set our current node pointer to the given scene node (root)
    current = newScene;
}

// returns new cloned instance, pointing at the specified node
template<class NodeType>
shared_ptr<Editor<NodeType>> Editor<NodeType>::node(const string& name){
    if(!current)
        return make_shared<Node>();

    // TODO: support for more specific component path
    NodeType n = current->getChildWithName(name);

    auto result = clone();
    result->setCurrent(n);

    return result;
}

// returns a new instance that is a clone of the current instance
template<class NodeType>
shared_ptr<Editor<NodeType>> Editor<NodeType>::clone(){
    auto c = make_shared<Editor<NodeType>>();
    c.clone(*this);
    return c;
}

// make the current instance a clone of the given instance
template<class NodeType>
void Editor<NodeType>::clone(const Editor<NodeType> &original){
    sceneData = original.getSceneData();
    current = original.getCurrent();
}
