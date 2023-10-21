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
    void updateParticleSystem();
    void drawParticleSystem();

    ParticleSystem particleSystem;
    
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
    
    int roomSelection, previousRoomSelection, runeSelection;
    float fuzzy, scale, follow, statusProgress;
    float simplexDepth, simplexOffset, simplexWrap, simplexPow, simplexAmount;
    
    float blur, bloomIntensity, bloomSpread, particleSize, depth;
    Boolean autoRotateFlag;
};
