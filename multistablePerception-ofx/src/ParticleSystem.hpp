//
//  ParticleSystem.hpp
//  multistablePerception
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include "ofMain.h"
#include "Particle.hpp"
#include <stdio.h>
#include <random>

// poission distribution
static random_device rd;
static mt19937 gen(rd());

class ParticleSystem {
public:
    ParticleSystem();
    
    void update();
    void draw();
    void drawFocusBox();
    
    ofVec3f getParticlePosition(int index);
    
    void addParticle(float x, float y, float lifetime);
    void setDepth(float _depth);
    void setSystem(int _whichSystem);
    void setScale(float _scale);
    void setFollow(float _follow);
    void setTwist(ofVec3f _twist);
    void setColor(ofColor _color);
    void setParticleSize(float _size);
    void setSimplexMorph(float _amount, float _depth, float _offset, float _wrap, float _pow);
    
    void setNumParticles(int numParticles);
    void setCircle(float x, float y, float radius);
    void setRectangle(float x, float y, float width, float height);
    void resizeSystem();
    void calculateMagnitudeExtents();
    
    // actions
    void random();
    void swap();
    void explode();
    int getNumParticles();
    
private:
    vector<Particle> particles;
    ofVec3f center, twist, min, max, ratios;
    float minMagnitude, maxMagnitude;

    ofColor primaryColor;
    
    float particleSize, scale, defaultRadius, focusSin, focusInc, targetScale;
    int numParticles, squareRootNumParticles, type, follow, depth;
        
    // initialization functions
    void setPoints(vector<ofVec3f> _points);
    void setDefaultRotation(float x, float y, float z);
    void centerPoints();
    void scalePoints();
    
    void resizeSystem(int _numParticles);
    void updateParticles();
    Particle getRandomParticle();
};

#endif /* ParticleSystem_hpp */
