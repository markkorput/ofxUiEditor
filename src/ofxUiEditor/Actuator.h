#pragma once

#include "ofxLambdaEvent/function.h"
#include "ofxCMS.h"
#include "NodeModel.h"

#define OFXUIEDITOR_DEFAULT_PROPERTIES_FILE "properties.json"

namespace ofxUiEditor {
    class PropsModel : public ofxCMS::Model {
    };

    //! \brief A class for updating a specific attribute of an arbitrary type of object using provided lambdas
    //!
    //! \description AttributeActuatorModel is a container for a (collection of) lambda(s) that know how to update a specific attribute from
    //! any kind ob object. It takes a string-based value as source data and calls custom registered (a) lambda(s) that perform the actuall operations on the target object.
    template<class BaseType>
    class AttributeActuatorModel : public ofxCMS::Model {
        public:
            typedef FUNCTION<void(shared_ptr<BaseType>, const string&)> ActuatorFunc;

        public:
            void actuate(shared_ptr<BaseType> instanceRef, const string& value){
                for(auto func : actuatorFuncs){
                    func(instanceRef, value);
                }
            }

            void addActuator(ActuatorFunc func){
                actuatorFuncs.push_back(func);
            }

        public:
            std::vector<ActuatorFunc> actuatorFuncs;
    };

    //! \brief A class for updating any arbitrary type of object using provided lambdas.
    //!
    //! \description TODO
    template<class BaseType>
    class ActuatorModel : public ofxCMS::Model {
        public: // types

            typedef FUNCTION<void(shared_ptr<BaseType>, const string&)> AttrActuatorFunc;

        public:
            void actuate(shared_ptr<BaseType> instanceRef, shared_ptr<PropsModel> propModelRef){
                // loop over each key/value pair (attribute) in the node model, and apply any attribute actuator we can find
                propModelRef->each([this, instanceRef](const string& attr, const string& val){
                    actuateAttribute(instanceRef, attr, val);
                });
            }

            void actuateAttribute(shared_ptr<BaseType> instanceRef, const string& attr, const string& value){
                // first apply all parents, effectively "inheriting" their actuator behaviour
                for(auto parentRef : parentRefs)
                    parentRef->actuateAttribute(instanceRef, attr, value);

                // find actuator for this attribute
                auto attrActuatorModelRef = this->attributeActuatorCollection.findById(attr);

                // if found, apply actuator
                if(attrActuatorModelRef)
                    attrActuatorModelRef->actuate(instanceRef, value);
            }

            void addParent(shared_ptr<ActuatorModel<BaseType>> parentRef){
                parentRefs.push_back(parentRef);
            }

            void addAttributeActuator(const string& attribute_name, AttrActuatorFunc func){
                auto attributeActuatorModelRef = attributeActuatorCollection.findById(attribute_name, true /* create-if-not-found */);
                attributeActuatorModelRef->addActuator(func);
            }

        private:
            std::vector<shared_ptr<ActuatorModel<BaseType>>> parentRefs;
            ofxCMS::Collection<AttributeActuatorModel<BaseType>> attributeActuatorCollection;
    };

    template<class BaseType>
    class Actuator {
        public:
            // typedef ActuatorModel<BaseType>::AttrActuatorFunc AttrActuatorFunc;
            typedef FUNCTION<void(shared_ptr<BaseType>, const string&)> AttrActuatorFunc;

        public:
            void setup();
            void addPropertiesFile(const string& filePath);

            void reload();

            void actuate(shared_ptr<BaseType> instanceRef, shared_ptr<ofxUiEditor::NodeModel> nodeModelRef, bool active=true);

            void addActuator(const string& identifier, const string& attribute_name, AttrActuatorFunc func){
                auto actuatorModelRef = actuatorCollection.findById(identifier, true /* create if not found */);
                actuatorModelRef->addAttributeActuator(attribute_name, func);
            }

            void addActuator(const string& identifier, const string& parent_identifier){
                auto actuatorModelRef = actuatorCollection.findById(identifier, true /* create if not found */);
                auto parentRef = actuatorCollection.findById(parent_identifier, true /* create if not found */);
                actuatorModelRef->addParent(parentRef);
            }

        private:
            std::vector<shared_ptr<BaseType>> createdInstancesRefs;
            ofxCMS::Collection<ActuatorModel<BaseType>> actuatorCollection;

            ofxCMS::Collection<PropsModel> propertiesCollection;
            std::set<string> loadedPropertiesFiles;
    };


    template<class BaseType>
    void Actuator<BaseType>::setup(){
        if(propertiesCollection.size() == 0 && ofFile::doesFileExist(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE))
            addPropertiesFile(OFXUIEDITOR_DEFAULT_PROPERTIES_FILE);

        // register some default actuators
        addActuator(".Node", "position_x", [](shared_ptr<BaseType> instanceRef, const string& value){
            instanceRef->setX(ofToFloat(value));
        });

        addActuator(".Node", "position_y", [](shared_ptr<BaseType> instanceRef, const string& value){
            instanceRef->setY(ofToFloat(value));
        });

        addActuator(".Node", "position_z", [](shared_ptr<BaseType> instanceRef, const string& value){
            instanceRef->setZ(ofToFloat(value));
        });

        addActuator(".Node", "size_x", [](shared_ptr<BaseType> instanceRef, const string& value){
            instanceRef->setWidth(ofToFloat(value));
        });

        addActuator(".Node", "size_y", [](shared_ptr<BaseType> instanceRef, const string& value){
            instanceRef->setHeight(ofToFloat(value));
        });

        addActuator(".Node", "scale_x", [](shared_ptr<BaseType> instanceRef, const string& value){
            ofVec3f vec3 = instanceRef->getScale();
            vec3.x = ofToFloat(value);
            instanceRef->setScale(vec3);
        });

        addActuator(".Node", "scale_y", [](shared_ptr<BaseType> instanceRef, const string& value){
            ofVec3f vec3 = instanceRef->getScale();
            vec3.y = ofToFloat(value);
            instanceRef->setScale(vec3);
        });

        addActuator(".Node", "scale_z", [](shared_ptr<BaseType> instanceRef, const string& value){
            ofVec3f vec3 = instanceRef->getScale();
            vec3.z = ofToFloat(value);
            instanceRef->setScale(vec3);
        });

        addActuator(".SolidColorPanel", ".Node"); // ".SolidColorPanel" copies all behaviour of the ".Node" Actuator
    }

    template<class BaseType>
    void Actuator<BaseType>::addPropertiesFile(const string& filePath){
        // remember which files we have loaded, for our .reload method
        loadedPropertiesFiles.insert(filePath);

        ofLog() << "Loading ofUiEditor properties file: " << filePath;
        propertiesCollection.loadJsonFromFile(filePath);
        // ofLog() << "PropertiesCollection size: "<<propertiesCollection.size();
    }

    template<class BaseType>
    void Actuator<BaseType>::reload(){
        for(auto& propFile : loadedPropertiesFiles){
            ofLog() << "Loading ofUiEditor properties file: " << propFile;
            propertiesCollection.loadJsonFromFile(propFile);
        }
    }

    template<class BaseType>
    void Actuator<BaseType>::actuate(shared_ptr<BaseType> instanceRef, shared_ptr<ofxUiEditor::NodeModel> nodeModelRef, bool active){
        std::vector<string> _ids;
        _ids.push_back("."+nodeModelRef->getClass());
        _ids.push_back(nodeModelRef->getId());

        // loop over and apply all relevant property models
        for(auto propModelRef : propertiesCollection.findByIds(_ids)){
            // apply using all relevant registered actuator models
            for(auto actuatorModelRef : actuatorCollection.findByIds(_ids)){
                actuatorModelRef->actuate(instanceRef, propModelRef);
            }

            if(active){
                propModelRef->attributeChangeEvent.addListener([this, _ids, instanceRef](PropsModel::AttrChangeArgs& args){
                    ofLogVerbose() << "Detected change to attribute `" << args.attr << "` of ofxUiEditor::PropsModel " << args.model->get("id") << ", actuating linked view-object";
                    // AttrChangeArgs:
                    //  Model *model;
                    //  string attr;
                    //  string value;

                    // find all currently registeredrelevant actuatorModels and apply them
                    for(auto actuatorModelRef : this->actuatorCollection.findByIds(_ids)){
                        actuatorModelRef->actuateAttribute(instanceRef, args.attr, args.value);
                    }
                }, this);
            }
        }
    }
}
