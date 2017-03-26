#pragma once

#include "MeshData.h"

namespace ofxUiEditor {
    class MeshDataManager {

    public: // common methods
        void draw();

    public: // getters/setters
        shared_ptr<MeshData> find(const string &id);
        shared_ptr<MeshData> get(const string &id);

    private: // callbacks
        void onItemChange(MeshData &item);

    public: // events
        ofEvent<MeshData> newItemEvent, itemUpdatedEvent;

    private: // attributes
        map<string, shared_ptr<MeshData>> items;
    };
};
