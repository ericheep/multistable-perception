//
//  ParticleSystem.hpp
//  multistable perception
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include "ofMain.h"
#include "Particle.hpp"
#include <stdio.h>
#include <random>

class ParticleSystem {
public:
    ParticleSystem();
    
    void update();
    void draw();
    void drawFocusBox();
    
    ofVec3f getParticlePosition(int index);
    
    void addParticle(float x, float y, float lifetime, Boolean persistence);
    void setIndex(int index);
    void setDepth(float _depth);
    void setScale(float _scale);
    void setFollow(float _follow);
    void setTwist(ofVec3f _twist);
    void setColor(ofColor _color);
    void setParticleSize(float _size);
    void setSimplexMorph(float _amount, float _depth, float _offset, float _wrap, float _pow);
    void setRandomParticleSizes(float min, float max);
    
    void setTargets(ofVec3f target);
    void setNumParticles(int numParticles);
    void setCircle(float x, float y, float radius, int numParticles);
    void setRectangle(float x, float y, float width, float height, int numParticles);
    void resizeSystem();
    
    // actions
    void dissipate();
    void random();
    void swap();
    void explode();
    int getNumParticles();
    int getIndex();
    
private:
    vector<Particle> particles;
    ofVec3f center, twist, min, max, ratios;
    float minMagnitude, maxMagnitude;

    ofColor primaryColor;
    
    float particleSize, scale, defaultRadius, targetScale;
    int numParticles, squareRootNumParticles, type, follow, depth, index;
    float simplexAmount, simplexDepth, simplexOffset, simplexWrap, simplexPow;
        
    // initialization functions
    void setPoints(vector<ofVec3f> _points);
    void setDefaultRotation(float x, float y, float z);
    void centerPoints();
    void scalePoints();
    
    void resizeSystem(int _numParticles);
    void updateParticles();
    
    Particle getRandomParticle();
    
    Boolean isChasing;
};

#endif /* ParticleSystem_hpp */
