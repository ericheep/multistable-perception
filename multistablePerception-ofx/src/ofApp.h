#pragma once

#define _USE_MATH_DEFINES
 
#include <cmath>
#include "ofxGui.h"
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofShader.h"

#include "ParticleSystem.hpp"

#define SENDING_PORT 6543
#define RECEIVING_PORT 5432

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
private:
    void updateOsc();
    void updatePointOfInterest();
    void updateParticleSystem();

    ParticleSystem particleSystem;
    vector<ParticleSystem> pools;
    
    ofShader bloom;
    ofShader blurX;
    ofShader blurY;
    ofShader feedback;
    
    ofFbo fboBloom;
    ofFbo fboBlurOnePass;
    ofFbo fboBlurTwoPass;
    ofFbo fboFeedback;
    ofFbo fboPreviousFrame;
    
    ofCamera cam;
    
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    ofVec2f rotationIncrement;
    ofVec2f rotation;
    ofVec2f targetRotation;
    ofVec3f twist;
    
    ofColor primaryColor;
    ofColor backgroundColor;
    
    float fuzzy, scale, follow, statusProgress;
    float blur, bloomIntensity, bloomSpread, particleSize, depth;
    Boolean autoRotateFlag;
};
