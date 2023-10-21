//
//  ParticleSystem.cpp
//  multistablePerception
//

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem() {
    scale = 1.0;
    particleSize = 1.5;
    defaultRadius = 500;
    ratios = ofVec3f(0.0, 0.0, 0.0);
    
    focusInc = 0.0;
    focusSin = 0.0;
    
    depth = 20;
}

void ParticleSystem::setNumParticles(int _numParticles) {
    resizeSystem(_numParticles);
}

void ParticleSystem::setCircle(float _x, float _y, float _radius) {
    resizeSystem(10000);

    for (int i = 0; i < numParticles; i++) {
        float magnitude = ofRandom(0, _radius);
        float angle = ofRandom(0, TWO_PI);
        
        float x = cos(angle) * magnitude + _x;
        float y = sin(angle) * magnitude + _y;
        float z = ofRandom(-depth / 2.0, depth / 2.0);
        
        // particles[i].setOrigin(ofVec3f(x, y, z));
    }
}

void ParticleSystem::addParticle(float _x, float _y, float _lifetime) {
    Particle particle = Particle(ofVec3f(_x, _y, 0), _lifetime);
    particles.push_back(particle);
    numParticles = int(particles.size());
}

void ParticleSystem::setRectangle(float _x, float _y, float _width, float _height) {
    resizeSystem(10000);
    
    for (int i = 0; i < numParticles; i++) {
        float x = ofRandom(0, _width) + _x;
        float y = ofRandom(0, _height) + _y;
        float z = ofRandom(-depth / 2.0, depth / 2.0);
        
        // particles[i].setOrigin(ofVec3f(x, y, z));
    }
}

void ParticleSystem::setColor(ofColor _primaryColor) {
    primaryColor = _primaryColor;
}

void ParticleSystem::update() {
    scale = ofLerp(scale, targetScale, 0.1);
    for (int i = 0; i < numParticles; i++) {
        particles[i].update();
    }
    for (int i = 0; i < numParticles; i++) {
        if (!particles[i].getState()) {
            particles.erase(particles.begin() + i);
        }
        numParticles = int(particles.size());
    }
}

void ParticleSystem::setFollow(float _follow) {
    for (int i = 0; i < numParticles; i++) {
        particles[i].setRandomFollow(_follow);
    }
}

void ParticleSystem::setDepth(float _depth) {
    depth = _depth;
    for (int i = 0; i < numParticles; i++) {
        ofVec3f point = particles[i].getOrigin();
        point.z = ofRandom(-_depth / 2.0, _depth / 2.0);
        
        particles[i].setOrigin(point);
    }
}

void ParticleSystem::setParticleSize(float s) {
    particleSize = s;
}

ofVec3f ParticleSystem::getParticlePosition(int _index) {
    return particles[_index].getPosition();
}

void ParticleSystem::setTwist(ofVec3f _twist) {
    for (int i = 0; i < numParticles; i++) {
        ofVec3f particleTwist(0, 0, 0);
        
        particleTwist.x = (float(i) / numParticles) * _twist.x;
        particleTwist.y = (float(i) / numParticles) * _twist.y;
        particleTwist.z = (float(i) / numParticles) * _twist.z;
        
        particles[i].setTwist(particleTwist);
    }
}

void ParticleSystem::draw() {    
    for (int i = 0; i < numParticles; i++) {
        particles[i].draw();
    }
}

void ParticleSystem::resizeSystem(int _numParticles) {
    numParticles = _numParticles;
    if (particles.size() > _numParticles) {
        while (particles.size() > _numParticles) {
            particles.pop_back();
        }
    }
    
    if (particles.size() < _numParticles) {
        while (particles.size() < _numParticles) {
            if (particles.size() > 0) {
                particles.push_back(getRandomParticle());
            } else {
                float x = ofRandomWidth();
                float y = ofRandomHeight();
                // particles.push_back(ofVec3f(x, y, 0));
            }
        }
    }
}

Particle ParticleSystem::getRandomParticle() {
    int randomIndex = int(ofRandom(0.0, particles.size()));
    return particles[randomIndex];
}

void ParticleSystem::setSimplexMorph(float _amount, float _depth, float _offset, float _wrap, float _pow) {
    for (int i = 0; i < numParticles; i++) {
        particles[i].setSimplexMorph(_amount, _depth, _offset, _wrap, _pow);
    }
}

void ParticleSystem::random() {
    for (int i = 0; i < numParticles; i++) {
        float x = ofRandomWidth();
        float y = ofRandomHeight();;
        float z = depth;
        
        particles[i].setPosition(ofVec3f(x, y, z));
    }
}

void ParticleSystem::swap() {
    for (int i = 0; i < numParticles; i++) {
        ofVec3f position = particles[i].getPosition();
        particles[i].setOrigin(-position);
    }
}

void ParticleSystem::setScale(float _scale) {
    targetScale = _scale;
    
    for (int i = 0; i < numParticles; i++) {
        particles[i].setScale(_scale);
    }
}

void ParticleSystem::scalePoints() {
    float distX = fabs(min.x - max.x);
    float distY = fabs(min.y - max.y);
    float distZ = fabs(min.z - max.z);
    
    float distMax = std::max({distX, distY, distZ});
    float scalar = defaultRadius / distMax;
    
    ratios.x = distX / distMax;
    ratios.y = distY / distMax;
    ratios.z = distZ / distMax;
    
    for (int i = 0; i < numParticles; i++) {
        ofVec3f o = particles[i].getOrigin();
        o = o * scalar;
        particles[i].setOrigin(o);
    }
}

int ParticleSystem::getNumParticles() {
    return int(particles.size());
}
