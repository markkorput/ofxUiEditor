#pragma once

#include "MeshData.h"

namespace ofxUiEditor {
    class MeshDataManager {

    public: // common methods
        void draw();
        void drawItem(shared_ptr<MeshData> item);

    public: // getters/setters
        shared_ptr<MeshData> find(const string &id);
        shared_ptr<MeshData> get(const string &id);

        vector<shared_ptr<MeshData>> getRootItems();
        vector<shared_ptr<MeshData>> getChildren(const string &parentId);

    private: // callbacks
        void onItemChange(MeshData &item);

    public: // events
        ofEvent<MeshData> newItemEvent, itemUpdatedEvent;

    private: // attributes
        map<string, shared_ptr<MeshData>> items;
    };
};
