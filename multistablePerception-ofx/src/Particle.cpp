//
//  Particle.cpp
//  multistablePerception
//

#include "Particle.hpp"
#include <random>

Particle::Particle(ofVec3f _point, float _lifetime) {
    maxParticleSize = 20;
    
    lifetime = sqrt(_lifetime);
    particleSize = _lifetime * maxParticleSize;
    
    origin.x = _point.x;
    origin.y = _point.y;
    origin.z = _point.z;
    
    position.x = origin.x;
    position.y = origin.y;
    position.z = origin.z;
    
    target = ofVec3f(0, 0, 0);
    morph = ofVec3f(0, 0, 0);
    
    follow = 0.5;
    
    simplexAmount = 0;
    simplexDepth = 0.004;
    simplexOffset = 0.001;
    simplexWrap = PI;
    simplexPow = 1;
    
    minMagnitude = 0.0;
    maxMagnitude = 0.0;
    
    off = 0.0;
    
    isAlive = true;
    
    primaryColor = ofColor::whiteSmoke;
    secondaryColor = ofColor::pink.getLerped(ofColor::blue, ofRandom(1.0));
}

Boolean Particle::getState() {
    return isAlive;
}

void Particle::setCenter(ofVec3f _center) {
    center = _center;
}

void Particle::setOrigin(ofVec3f _origin) {
    origin = _origin;
}

void Particle::setScale(float _scale) {
    scale = _scale;
}

ofVec3f Particle::getOrigin() {
    return origin;
}

void Particle::setTarget(ofVec3f _tar) {
    target = _tar;
}

void Particle::setNormal(ofVec3f _normal) {
    normal = _normal;
}

void Particle::setRotation(ofVec3f _rotation) {
    rotation = _rotation;
}

void Particle::setTwist(ofVec3f _twist) {
    twist = _twist;
}

ofVec3f Particle::getNormal() {
    return normal;
}

void Particle::setRandomFollow(float _randomFollow) {
    follow = ofRandom(_randomFollow, _randomFollow * 2.0);
}

void Particle::updatePosition() {
    position = position.getInterpolated(target, follow);
}

void Particle::updateSimplexMorph() {
    float x = origin.x * simplexDepth + off;
    float y = origin.y * simplexDepth + off;
    float z = origin.z * simplexDepth + off;
    off += simplexOffset;
    
    float mappedX = ofMap(ofNoise(x), 0.0, 1.0, -1.0, 1.0);
    float mappedY = ofMap(ofNoise(y), 0.0, 1.0, -1.0, 1.0);
    float mappedZ = ofMap(ofNoise(z), 0.0, 1.0, -1.0, 1.0);
    
    noise = ofNoise(x, y, z);
    float simplexMagnitude = pow(noise, simplexPow);
    
    float rotX = mappedX * simplexWrap;
    float rotY = mappedY * simplexWrap;
    float rotZ = mappedZ * simplexWrap;
    
    float length = simplexMagnitude * simplexAmount;
    
    ofVec3f v(length, length, length);
    v.rotateRad(rotX, ofVec3f(1.0, 0.0, 0.0));
    v.rotateRad(rotY, ofVec3f(0.0, 1.0, 0.0));
    v.rotateRad(rotZ, ofVec3f(0.0, 0.0, 1.0));

    morph.x = v.x;
    morph.y = v.y;
    morph.z = v.z;
}

void Particle::updateTwist() {
    twisted = origin.getRotatedRad(twist.x, twist.y, twist.z);
}

void Particle::updateTarget() {
    target = (twisted + morph) * scale;
}

void Particle::updateLifetime() {
    particleSize = lifetime * maxParticleSize;
    lifetime -= 0.005;
    particleColor = primaryColor.getLerped(secondaryColor, lifetime);
    
    if (lifetime < 0) {
        isAlive = false;
    }
}

void Particle::draw() {
    ofFill();
    ofSetColor(particleColor);
    ofDrawSphere(position, particleSize);
}

void Particle::update(){
    updateLifetime();
    updateTwist();
    updateSimplexMorph();
    updateTarget();
    updatePosition();
}

void Particle::setSimplexMorph(float _amount, float _depth, float _offset, float _wrap, float _pow) {
    simplexAmount = _amount;
    simplexDepth = _depth;
    simplexOffset = _offset;
    simplexWrap = _wrap;
    simplexPow = _pow;
}

void Particle::setPosition(ofVec3f _position) {
    position = _position;
}

ofVec3f Particle::getPosition() {
    return position;
}
