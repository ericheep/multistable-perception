#pragma once

#define _USE_MATH_DEFINES
 
#include <cmath>
#include "ofxGui.h"
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofShader.h"
#include "ofxQuadWarp.h"

#include "ParticleSystem.hpp"

#define SENDING_PORT 6543
#define RECEIVING_PORT 5432

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    
    ofxQuadWarp warper;
private:
    void updateOsc();
    void updatePointOfInterest();
    void updateParticleSystem();

    ParticleSystem particleSystem;
    vector<ParticleSystem> pools;
    
    
    ParticleSystem speakerTest;
    ParticleSystem zipper;
    ParticleSystem rotatingRocks;
    ParticleSystem iceColumns;
    ParticleSystem blips;
    ParticleSystem stasis;
    ParticleSystem vinylShifts;
    ParticleSystem waves;
    
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
    ofVec3f stasisTwist;
    
    ofColor primaryColor;
    ofColor backgroundColor;
    
    ofColor amy;
    ofColor danny;
    ofColor ethan;
    ofColor eric;
    ofColor joe;
    vector<ofColor> rachel;
    
    ofPoint points[10];
    
    float fuzzy, scale, follow, statusProgress, stasisRotation, blipsRotation;
    float blur, bloomIntensity, bloomSpread, particleSize, depth;
    
    Boolean autoRotateFlag;
};
