#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(TRUE);
    ofSetFrameRate(60);
    
    // gl settings
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    
    // osc settings
    oscSender.setup("localhost", SENDING_PORT);
    oscReceiver.setup(RECEIVING_PORT);
    
    // load shaders
    bloom.load("shaders/bloom");
    blurX.load("shaders/blurX");
    blurY.load("shaders/blurY");
    // feedback.load("shaders/feedback");
    
    // load frame buffer objects
    fboBloom.allocate(ofGetWidth(), ofGetHeight());
    fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
    fboFeedback.allocate(ofGetWidth(), ofGetHeight());
    // fboPreviousFrame.allocate(ofGetWidth(), ofGetHeight());
    
    // initialize colors
    backgroundColor = ofColor(0, 0, 0);
    primaryColor = ofColor::white;
    
    // initialize vectors
    rotationIncrement = ofVec2f(0.001, 0.002);
    rotation = ofVec2f(0.0, 0.0);
    targetRotation = ofVec2f(0.0, 0.0);
    twist = ofVec3f(0.0, 0.0, 0.0);
    
    scale = 1.0;
    follow = 0.04;
    particleSize = 2.0;

    simplexAmount = 150.0;
    simplexDepth = 0.004;
    simplexOffset = 0.001;
    simplexPow = 1.0;
    simplexWrap = TWO_PI;
    
    blur = 0.5;
    bloomSpread = 0.7;
    bloomIntensity = 1.1;
    
    autoRotateFlag = true;
    
    // initialize classes
    // particleSystem.setRectangle(0, 0, 100, ofGetHeight());
    // particleSystem.setNumParticles(0);
    // particleSystem.setCircle(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 300);
    // particleSystem.setColor(primaryColor);
    
    // initialize cam
    cam.setupPerspective();
}

void ofApp::updateParticleSystem() {
    particleSystem.setSimplexMorph(simplexAmount, simplexDepth, simplexOffset, simplexWrap, simplexPow);
    particleSystem.setScale(scale);
    particleSystem.setFollow(follow);
    particleSystem.setTwist(twist);
    particleSystem.update();
}

//--------------------------------------------------------------
void ofApp::update() {
    updateOsc();
    updateParticleSystem();
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    // set up frame buffer objects for multiple shadder passes
    fboFeedback.begin();
    ofClear(0, 0, 0);
    
    // non-feedback fbos
    fboBlurTwoPass.begin();
    ofClear(0, 0, 0);
    fboBlurOnePass.begin();
    ofClear(0, 0, 0);
    fboBloom.begin();
    ofClear(0, 0, 0);
    
    // draw
    ofBackground(backgroundColor);
    ofEnableDepthTest();
    drawParticleSystem();
    ofDisableDepthTest();
    fboBloom.end();
    
    // bloom shader
    bloom.begin();
    bloom.setUniform1f("bloom_spread", bloomSpread);
    bloom.setUniform1f("bloom_intensity", bloomIntensity);
    fboBloom.draw(0, 0);
    bloom.end();
    fboBlurOnePass.end();
    
    // gaussian first pass blurX shader
    blurX.begin();
    blurX.setUniform1f("blurAmnt", blur);
    fboBlurOnePass.draw(0, 0);
    blurX.end();
    fboBlurTwoPass.end();
    
    // guassian second pass blurY shader
    blurY.begin();
    blurY.setUniform1f("blurAmnt", blur);
    fboBlurTwoPass.draw(0, 0);
    blurY.end();
    fboFeedback.end();
    
    // small float incrementer
    // 100 frames per cycle
    // float scalarTime = fmod(ofGetFrameNum() * 0.01, 1.0);
    
    // feedback shader
    //feedback.begin();
    //feedback.setUniform1f("u_feedback", feedbackSlider);
    //feedback.setUniform1f("u_offset", offsetSlider);
    //feedback.setUniform1f("u_time", scalarTime);
    //feedback.setUniformTexture("tex1", fboPreviousFrame.getTexture(0), 0);
    //feedback.end();
    
    //fboFeedback.draw(0, 0);
    //fboPreviousFrame.end();
    
    fboFeedback.getTexture().getTextureData().bFlipTexture = true;
    fboFeedback.draw(0, 0);
    
    cam.end();
    
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

void ofApp::drawParticleSystem() {
    //ofPushMatrix();
    //ofTranslate(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
    //ofRotateXRad(rotation.x);
    //ofRotateYRad(rotation.y);
    
    particleSystem.draw();
    
   // ofPopMatrix();
}

void ofApp::updateOsc() {
    while(oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        
        if (m.getAddress() == "/particle") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();

            particleSystem.addParticle(x, y, intensity);
        }
        
        if (m.getAddress() == "/coordinates") {
            float x = m.getArgAsFloat(0);
            float y = m.getArgAsFloat(1);
            float width = m.getArgAsFloat(2);
            float height = m.getArgAsFloat(3);
            
            x *= ofGetWidth();
            y *= ofGetHeight();
            width *= ofGetWidth();
            height *= ofGetHeight();
            
            particleSystem.setRectangle(x, y, width, height);
        }
        
        if (m.getAddress() == "/particleSize") {
            float particleSize = m.getArgAsFloat(0);
            particleSystem.setParticleSize(particleSize);
        }
        
        if (m.getAddress() == "/depth") {
            depth = m.getArgAsFloat(0);
            particleSystem.setDepth(depth);
        }
        
        if (m.getAddress() == "/blur") {
            blur = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/bloomIntensity") {
            bloomIntensity = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/bloomSpread") {
            bloomSpread = m.getArgAsFloat(0);
        }

        if (m.getAddress() == "/simplexDepth") {
            simplexDepth = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/simplexOffset") {
            simplexOffset = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/simplexWrap") {
            simplexWrap = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/simplexPow") {
            simplexPow = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/simplexAmount") {
            simplexAmount = m.getArgAsFloat(0);
        }
    }
}
