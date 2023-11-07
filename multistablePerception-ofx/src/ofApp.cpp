#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(TRUE);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    
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
    blips.setSimplexMorph(5, 0.0001, 0.000001, TWO_PI, 1.0);

    // initialize cam
    cam.setupPerspective();
    
    // top left
    int x1 = 50;
    int y1 = 50;
    // top right
    int x2 = ofGetWidth() - 50;
    int y2 = 50;
    // bottom left
    int x3 = 50;
    int y3 = ofGetHeight() - 50;
    // bottom right
    int x4 = ofGetWidth() - 50;
    int y4 = ofGetHeight() - 50;
    
    warper.setSourceRect(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    
    warper.setTopLeftCornerPosition(ofPoint(x1, y1));
    warper.setTopRightCornerPosition(ofPoint(x2, y2));
    warper.setBottomLeftCornerPosition(ofPoint(x3, y3));
    warper.setBottomRightCornerPosition(ofPoint(x4, y4));

    warper.setup();
    warper.load();
    
    warper.hide();

    amy.set(237, 255, 217);
    danny.set(252, 81, 63);
    ethan.set(181, 220, 244);
    joe.set(170, 197, 169);
    eric.set(255, 213, 220);
    amy.setSaturation(100);
    ethan.setSaturation(100);
    joe.setSaturation(100);

    rachel.push_back(ofColor(249, 223, 249));
    rachel.push_back(ofColor(246, 240, 255));
    rachel.push_back(ofColor(224, 223, 248));
    rachel.push_back(ofColor(218, 199, 248));
    rachel.push_back(ofColor(193, 190, 255));
    
    rachel[0].setSaturation(80);
    rachel[1].setSaturation(80);
    rachel[2].setSaturation(80);
    rachel[3].setSaturation(80);
    rachel[4].setSaturation(80);
    
    speakerTest.setColor(ofColor::whiteSmoke);
    zipper.setColor(ethan);
    rotatingRocks.setColor(amy);
    iceColumns.setColor(joe);
    blips.setColor(eric);
    vinylShifts.setColor(danny);
    waves.setColor(ofColor::whiteSmoke);
    stasis.setScale(0.5);
    blips.setScale(0.5);
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
    
    speakerTest.update();
    zipper.update();
    rotatingRocks.update();
    iceColumns.update();
    blips.setFollow(follow);
    blips.setScale(scale);
    blips.setTwist(twist);
    blips.update();

    stasis.update();
    vinylShifts.update();
    waves.update();
    stasis.setTwist(stasisTwist);

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(backgroundColor);
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
    
    // particle systems
    speakerTest.draw();
    zipper.draw();
    rotatingRocks.draw();
    iceColumns.draw();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
    ofRotateXRad(blipsRotation);
    blips.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
    ofRotateZRad(stasisRotation);
    stasis.draw();
    ofPopMatrix();
    
    vinylShifts.draw();
    waves.draw();
    
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
    
    fboFeedback.getTexture().getTextureData().bFlipTexture = true;
    
    ofMatrix4x4 warpMatrix = warper.getMatrix();
    
    ofPushMatrix();
    ofMultMatrix(warpMatrix);
    ofSetColor(255, 255, 255, 255);
    fboFeedback.draw(0, 0);
    ofPopMatrix();
    
    ofSetLineWidth(2);
    ofSetColor(ofColor::cyan);
    
    for(int i=0; i<9; i++) {
        int j = i + 1;
        
        ofVec3f p1 = warpMatrix.preMult(ofVec3f(points[i].x, points[i].y, 0));
        ofVec3f p2 = warpMatrix.preMult(ofVec3f(points[j].x, points[j].y, 0));
        
        ofDrawLine(p1.x, p1.y, p2.x, p2.y);
    }
    
    ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();
        
    cam.end();
    
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

void ofApp::keyPressed(int key) {
    if(key == 's' || key == 'S') {
        warper.toggleShow();
    }
    
    if(key == 'l' || key == 'L') {
        warper.load();
    }
    
    if(key == 'h' || key == 'H') {
        warper.save();
    }
}

void ofApp::exit() {
    warper.save();
}

void ofApp::updateOsc() {
    while(oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        
        if (m.getAddress() == "/speakerTest") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            speakerTest.addParticle(x, y, .5, false);
        }
        
        if (m.getAddress() == "/zipper") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            
            zipper.setSimplexMorph(150.0, 0.004, 0.004, TWO_PI, 1.0);
            zipper.setScale(scale);
            zipper.setFollow(follow);
            zipper.addParticle(x, y, intensity, false);
        }
        
        if (m.getAddress() == "/rotatingRocks") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            
            rotatingRocks.setSimplexMorph(50.0, 0.004, 0.004, TWO_PI, 1.0);
            rotatingRocks.setScale(scale);
            rotatingRocks.setFollow(follow);
            rotatingRocks.addParticle(x, y, intensity, false);
        }
        
        
        if (m.getAddress() == "/iceColumns") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            
            iceColumns.addParticle(x, y, .5, false);
        }
        
        if (m.getAddress() == "/blips") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            ofColor randomColor = rachel[int(ofRandom(rachel.size()))];
            blips.setColor(randomColor);
            blips.addParticle(x, y, .25, true);
            blips.setSimplexMorph(10.0, 0.004, 0.001, TWO_PI, 1.0);
            blips.setRandomParticleSizes(2.0, 4.0);
        }
        
        if (m.getAddress() == "/blipsRotation") {
            blipsRotation = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/blipsDissipate") {
            blips.dissipate();
        }
        
        if (m.getAddress() == "/stasis") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
           
            stasis.addParticle(x, y, .3, true);
            stasis.setSimplexMorph(50.0, 0.004, 0.004, TWO_PI, 1.0);
        }
        
        if (m.getAddress() == "/stasisRotation") {
            stasisRotation = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/stasisTwist") {
            float r = m.getArgAsFloat(0);
            r = r * TWO_PI;
            ofVec3f stasisTwist = ofVec3f(ofRandom(-r, r), ofRandom(-r, r), ofRandom(-r, r));
            // stasis.setTwist(twist);
        }
        
        if (m.getAddress() == "/stasisDissipate") {
            stasis.dissipate();
        }
        
        if (m.getAddress() == "/vinylShifts") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            
            vinylShifts.setSimplexMorph(30.0, 0.004, 0.004, TWO_PI, 1.0);
            vinylShifts.setScale(scale);
            vinylShifts.setFollow(follow);
            vinylShifts.addParticle(x, y, intensity, false);
        }
        
        if (m.getAddress() == "/waves") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            
            waves.addParticle(x, y, .25, false);
        }
        
        if (m.getAddress() == "/particle") {
            float intensity = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1) * ofGetWidth();
            float y = m.getArgAsFloat(2) * ofGetHeight();
            
            particleSystem.addParticle(x, y, intensity, false);
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
