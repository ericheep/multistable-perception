//
//  Particle.hpp
//  multistablePerception
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <ofMain.h>

class Particle {
public:
    Particle();
    Particle(ofVec3f point);
    Particle(ofVec3f point, Boolean isPersistent, float lifetime);
    
    void setOrigin(ofVec3f _origin);
    ofVec3f getOrigin();
    
    void setPosition(ofVec3f _position);
    ofVec3f getPosition();

    void setNormal(ofVec3f _normal);
    ofVec3f getNormal();
    
    void setParticleSize(float particleSize);
    float getParticleSize();
    
    void setTarget(ofVec3f _target);
    void setTwist(ofVec3f twist);
    void setRotation(ofVec3f rotation);
    void setShape(ofVec3f shape);
    void setCenter(ofVec3f center);
   
    void setPersistence(Boolean persistence);
    void setLifetime(float lifetime, float lifetimeSpeed);
    
    void setScale(float scale);
    void setFuzzy(float fuzzy);
    void setPer(float per);
    void setSimplexMorph(float rate, float depth, float offset, float wrap, float pow);
    void setRandomFollow(float randomFollow);
        
    Boolean getState();
    
    void draw();
    void update();
private:
    float follow, fuzzy, off, scale, magnitude, noise, minMagnitude, maxMagnitude, lifetime, lifetimeSpeed;
    float simplexAmount, simplexDepth, simplexOffset, simplexWrap, simplexPow, particleSize, maxParticleSize;
    
    ofVec3f origin, position, target, morph, normal, center, rotation, twist, twisted, simplexTarget;
    ofColor primaryColor, secondaryColor;
    ofColor particleColor;
    
    void updateLifetime();
    void updateSimplexMorph();
    void updateTwist();
    void updateSimplexTarget();
    void updateOrigin();
    void updateTarget();
    void updatePosition();
    
    Boolean isAlive;
    Boolean isPersistent;
};

#endif /* Particle_hpp */
