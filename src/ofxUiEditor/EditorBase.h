#pragma once

#include "ofxInterface.h"
#include "ofxCMS.h"
#include "ofxLambdaEvent/LambdaEvent.h"

#include "StructureManager.h"
#include "PropertiesActuators.h"
#include "NodeModel.h"
#include "macros.h"

#define OFXUIEDITOR_DEFAULT_STRUCTURE_FILE "structures.xml"
#define OFXUIEDITOR_DEFAULT_PROPERTIES_FILE "properties.json"

namespace ofxUiEditor {

    class PropertiesModel : public ofxCMS::Model {
    };

    class EditorBase {

    public:
        void setup();
        shared_ptr<NodeModel> get(const string& nodePath, bool recursive=true);
        void addPropertiesFile(const string& filePath);

    private:
        StructureManager structureManager;
        ofxCMS::Collection<PropertiesModel> propertiesCollection;
        std::set<string> loadedPropertiesFiles;
    };
}
