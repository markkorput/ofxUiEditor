#pragma once

#include "MeshData.h"

namespace ofxUiEditor {
    class MeshDataManager {

    public: // getters/setters
        MeshData* find(const string &id);
        MeshData* get(const string &id);

    private: // callbacks
        void onItemChange(MeshData &item);

    public: // events
        ofEvent<MeshData> newItemEvent, itemUpdatedEvent;

    private: // attributes
        map<string, MeshData> items;
    };
};
