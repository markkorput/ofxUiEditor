#pragma once

#include "ofxCMS.h"
#include "EditorBase.h"

#ifdef OFXUIEDITOR_ANIMATIONS
    #include "ofxAnimatable.h"
#endif

namespace ofxUiEditor {

    class AnimationModel : public ofxCMS::Model {
        // nothing custom for now...
    };

    class Animator {
        public:
            Animator() : editorBase(NULL) {}

        private:
            EditorBase* editorBase;
            ofxCMS::Collection<AnimationModel> animationCollection;

    };
}
