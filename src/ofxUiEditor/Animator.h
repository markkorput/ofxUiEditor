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
            void setup();
            void update(float dt);

            //Animator();
            shared_ptr<AnimationFloat> startAnimation(const string& identifier);

        private:
            ofxCMS::Collection<AnimationModel> animationCollection;
            std::vector<shared_ptr<AnimationFloat>> activeAnimationRefs;

    };
}
