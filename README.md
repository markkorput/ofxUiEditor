# ofxUiEditor
OF addon intended as extension to ofxInterface that provides tools for faster UI-layout design and implementation.

## Usage - Basic

Create a layout (similar to a website's html file)

structures.xml
```xml
<structures>
    <window>
        <titlebar>
            <title />
            <close />
        </titlebar>
        <message />
        <cancel />
        <submit />
    </window>
</structures>
```

Create property definitions (similar to a website's CSS stylesheets)

properties.json
```json
{
    "window": {
        "width": 300,
        "height": 200,
        "position_x": 123,
        "position_y": 456,
        "position_z": 789,
        "scale_x": 0.5,
        "scale_y": 0.25,
        "scale_z": 0.1
    }
}
```

Use ofxUiEditor to generate the node

```c++
// ... TODO ...
auto windowNodeRef = uiEditor.create("window");
```

## Usage - Custom Properties for our CustomerProgressBar

structures.xml
```xml
<structures>
    <popupDialog>
        <CustomProgressBar />
    </popupDialog>
</structures>
```

properties.json
```json
{
    "popupDialog/CustomProgressBar": {
        "size_x": 400,
        "size_y": 50,
        "empty-color_r": 150,
        "empty-color_g": 150,
        "empty-color_b": 150,
        "full-color_r": 0,
        "full-color_g": 255,
        "full-color_b": 0
    }
}
```

CustomProgressBarComponent.hpp

```c++
class CustomProgressBarComponent : ofxInterface::Node {
    // make the actuator class a friend, so we don't have to create
    // getter and setter methods for all private property variables
    friend CustomProgressBarComponentActuator;

private: // attribute
    float progress;
    ofColor emptyColor, fullColor;

public:
    drawCustomProgressBarComponent() : progress(0.0f){
    }

    void draw(){
        ofSetColor(emptyColor);
        ofDrawRectangle(0.0f, 0.0f, getWidth(), getHeight());
        ofSetColor(fullColor);
        ofDrawRectangle(0.0f, 0.0f, getWidth() * progress, getHeight());
    }

    void setProgress(float newProgress){
        progress = newProgress;
    }
};
```

CustomProgressBarComponentPropertiesActuator.hpp
```c++
class CustomProgressBarComponentPropertiesActuator : ofxUiEditor::BasePropertiesActuator<ofxInterface::Node> {
    void actuate(shared_ptr<ofxInterface::Node> nodeRef, shared_ptr<ofxUiEditor::PropertiesItem> propertiesRef){
        auto progressBarRef = (shared_ptr<CustomProgressBarComponent>) nodeRef;
        // let our parent class take care of the default properties (size, position, scale, rotation)
        ofxUiEditor::BasePropertiesActuator::actuate(progressBarRef, propertiesRef);
        progressBarRef->emptyColor = propertiesRef->get("empty-color", ofColor(0.0f));
        progressBarRef->fullColor = propertiesRef->get("full-color", ofColor(255.0f));
    }
};
```

SomeApplication.cpp
```c++
// ...
uiEditor.addComponentPropertiesActuator("popupDialog/CustomProgressBar", make_shared<CustomProgressBarComponentPropertiesActuator>());
```
