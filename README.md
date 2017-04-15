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


## Usage - Custom Properties

```c++
    
```
