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
    follow = 0.06;
    particleSize = 2.0;

    blur = 0.5;
    bloomSpread = 0.7;
    bloomIntensity = 1.1;
    
    autoRotateFlag = true;
    
    // initialize systems
    particleSystem.setSimplexMorph(150, 0.004, 0.00025, TWO_PI, 1.0);

    // initialize cam
    cam.setupPerspective();
}

void ofApp::updatePointOfInterest() {
    for (int i = 0; i < pools.size(); i++) {
        pools[i].setScale(scale);
        pools[i].setTwist(twist);
        pools[i].update();
        
        if (pools[i].getNumParticles() == 0) {
            pools.erase(pools.begin() + i);
        }
    }
}

void ofApp::updateParticleSystem() {
    particleSystem.setScale(scale);
    particleSystem.setTwist(twist);
    particleSystem.update();
}

//--------------------------------------------------------------
void ofApp::update() {
    updateOsc();
    updateParticleSystem();
    updatePointOfInterest();
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
    particleSystem.draw();
    for (int i = 0; i < pools.size(); i++) {
        pools[i].draw();
    }
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
            
            particleSystem.setRectangle(x, y, width, height, 300);
        }
        
        if (m.getAddress() == "/particleSize") {
            float particleSize = m.getArgAsFloat(0);
            particleSystem.setParticleSize(particleSize);
        }
        
        if (m.getAddress() == "/addPool") {
            int index = m.getArgAsInt(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();

            ParticleSystem pool;
            pool.setIndex(index);
            pool.setCircle(x, y, ofRandom(100, 200), 1500);
            pool.setSimplexMorph(50.0, 0.004, 0.004, TWO_PI, 1.0);
            pool.setRandomParticleSizes(2.0, 4.0);
            pool.setFollow(0.01);
            
            pools.push_back(pool);
        }
        
        if (m.getAddress() == "/dissipatePool") {
            int index = m.getArgAsInt(0);
            
            for (int i = 0; i < pools.size(); i++) {
                if (pools[i].getIndex() == index) {
                    pools[i].dissipate();
                }
            }
        }
    }
}
