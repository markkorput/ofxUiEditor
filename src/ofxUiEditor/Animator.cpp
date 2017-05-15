#include "Animator.h"

#define OFXUIEDITOR_DEFAULT_ANIMATIONS_FILE ("animations.json")

using namespace ofxUiEditor;

void AnimationFloat::setup(shared_ptr<AnimationModel> animModelRef){
    #ifdef OFXUIEDITOR_OFXANIMATABLE
        anim.setDuration(ofToFloat(animModelRef->get("duration", "1.0")));
        // anim.setCurve // TODO
        anim.animateFromTo(
            ofToFloat(animModelRef->get("from", "0.0")),
            ofToFloat(animModelRef->get("to", "1.0")));
    #endif
}

bool AnimationFloat::update(float dt){
    #ifdef OFXUIEDITOR_OFXANIMATABLE
        if(!anim.isOrWillBeAnimating())
            return false;

        float beforeValue = anim.val();

        anim.update(dt);
        float afterValue = anim.val();

        if(afterValue != beforeValue)
            updateEvent.notifyListeners(afterValue);

        if(!anim.isOrWillBeAnimating()){
            doneEvent.notifyListeners(*this);
            return false; // not active anymore
        }

        return true;
    #else
        return false;
    #endif
}

AnimationFloat* AnimationFloat::onUpdate(UpdateFunc func){
    updateEvent += func;
    return this;
}

AnimationFloat* AnimationFloat::whenDone(DoneFunc func){
    doneEvent.addListener([func](AnimationFloat& anim){
        func();
    }, this);

    return this;
}


void Animator::setup(){
    string filePath = OFXUIEDITOR_DEFAULT_ANIMATIONS_FILE;

    if(animationCollection.size() == 0 && ofFile::doesFileExist(filePath)){
        ofLog() << "Loading animations data from: " << filePath;
        animationCollection.loadJsonFromFile(filePath);
        ofLogVerbose() << "Loaded " << animationCollection.size() << " animations";
    }
}

void Animator::update(float dt){
    std::vector<int> removeIndexes;
    int idx=0;

    for(auto animRef : activeAnimationRefs){
        if(!animRef->update(dt))
            removeIndexes.push_back(idx);

        idx++;
    }

    for(int removeIdx : removeIndexes)
        activeAnimationRefs.erase(activeAnimationRefs.begin()+removeIdx);
}

shared_ptr<AnimationFloat> Animator::startAnimation(const string& identifier){
    auto animRef = make_shared<AnimationFloat>();

    auto animModelRef = animationCollection.findById(identifier);

    if(animModelRef){
        animRef->setup(animModelRef);
    } else {
        ofLogWarning() << "unknown animation: " << identifier;
    }

    activeAnimationRefs.push_back(animRef);
    return animRef;
}
