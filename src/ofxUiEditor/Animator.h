#pragma once

#include "ofxCMS.h"
#include "ofxLambdaEvent.h"
#include "EditorBase.h"

#ifdef OFXUIEDITOR_OFXANIMATABLE
    #include "ofxAnimatableFloat.h"
#endif

namespace ofxUiEditor {

    class AnimationModel : public ofxCMS::Model {
        // nothing custom for now...
    };

    class AnimationFloat {
    public:
        typedef FUNCTION<void(float)> UpdateFunc;
        typedef FUNCTION<void()> DoneFunc;

    public:
        void setup(shared_ptr<AnimationModel> animModelRef);
        // updates insternal animation instance and returns if the animation is still active
        bool update(float dt);

        AnimationFloat* onUpdate(UpdateFunc func);
        AnimationFloat* whenDone(DoneFunc func);

    public:
        LambdaEvent<float> updateEvent;
        LambdaEvent<AnimationFloat> doneEvent;

    private:
        #ifdef OFXUIEDITOR_OFXANIMATABLE
            ofxAnimatableFloat anim;
        #endif
    };

    class Animator {
        public:
            //! loads the default animations data file: "animations.json"
            void setup();

            //! updates all active animations and removes finished animations from internal memory
            void update(float dt);
            void reload();

            //! creates and starts a new animation based on data from the json file
            shared_ptr<AnimationFloat> startAnimation(const string& identifier);

            size_t activeAnimationCount() const { return activeAnimationRefs.size(); }

        private:
            //! this collection holds the data loaded from data files. Every model basically represents an animation template.
            ofxCMS::Collection<AnimationModel> animationCollection;

            //! this list holds all currently active animations
            std::vector<shared_ptr<AnimationFloat>> activeAnimationRefs;
    };
}
