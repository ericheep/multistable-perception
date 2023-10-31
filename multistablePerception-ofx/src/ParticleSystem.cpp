//
//  ParticleSystem.cpp
//  multistablePerception
//

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem() {
    scale = 1.0;
    particleSize = 1.5;
    defaultRadius = 500;
    depth = 50;
    
    center = ofVec3f(0.0, 0.0, 0.0);
    ratios = ofVec3f(0.0, 0.0, 0.0);
    
    isChasing == false;
}

void ParticleSystem::setIndex(int _index) {
    index = _index;
}

int ParticleSystem::getIndex() {
    return index;
}

void ParticleSystem::setRandomParticleSizes(float min, float max) {
    for (int i = 0; i < numParticles; i++) {
        particles[i].setParticleSize(ofRandom(min, max));
    }
}

void ParticleSystem::setNumParticles(int _numParticles) {
    resizeSystem(_numParticles);
}

void ParticleSystem::setCircle(float _x, float _y, float _radius, int _numParticles) {
    resizeSystem(_numParticles);
    center.x = _x;
    center.y = _y;
    
    for (int i = 0; i < numParticles; i++) {
        float magnitude = ofRandom(0, _radius);
        float angle = ofRandom(0, TWO_PI);
        
        float x = cos(angle) * magnitude + _x;
        float y = sin(angle) * magnitude + _y;
        float z = ofRandom(-depth / 2.0, depth / 2.0);
        
        particles[i].setPosition(ofVec3f(_x, _y, z));
        particles[i].setOrigin(ofVec3f(x, y, z));
    }
}

void ParticleSystem::setTargets(ofVec3f target) {
    for (int i = 0; i < numParticles; i++) {
        particles[i].setTarget(target);
    }
}

void ParticleSystem::addParticle(float _x, float _y, float _lifetime) {
    float z = ofRandom(-depth / 2.0, depth / 2.0);
    ofVec3f point = ofVec3f(_x, _y, z);
    Particle particle = Particle(point, false, _lifetime);
    particle.setSimplexMorph(simplexAmount, simplexDepth, simplexOffset, simplexWrap, simplexPow);
    particle.setRandomFollow(follow);
    particles.push_back(particle);
    
    numParticles = int(particles.size());
    
    if (isChasing) {
        setTargets(point);
    }
}

void ParticleSystem::setRectangle(float _x, float _y, float _width, float _height, int _numParticles) {
    resizeSystem(_numParticles);
    center.x = _x;
    center.y = _y;
    
    for (int i = 0; i < numParticles; i++) {
        float x = ofRandom(0, _width) + _x;
        float y = ofRandom(0, _height) + _y;
        float z = ofRandom(-depth / 2.0, depth / 2.0);
        
        particles[i].setOrigin(ofVec3f(_x, _y, z));
        particles[i].setOrigin(ofVec3f(x, y, z));
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
            numParticles = int(particles.size());
        }
    }
}

void ParticleSystem::setFollow(float _follow) {
    follow = _follow;
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

void ParticleSystem::setParticleSize(float _particleSize) {
    particleSize = _particleSize;
    for (int i = 0; i < numParticles; i++) {
        particles[i].setParticleSize(particleSize);
    }
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
                float z = ofRandom(-depth / 2.0, depth / 2.0);
                
                ofVec3f point = ofVec3f(x, y, z);
                Particle particle = Particle(point, true, 1000);
                
                particles.push_back(particle);
            }
        }
    }
}

Particle ParticleSystem::getRandomParticle() {
    int randomIndex = int(ofRandom(0.0, particles.size()));
    return particles[randomIndex];
}

void ParticleSystem::setSimplexMorph(float _amount, float _depth, float _offset, float _wrap, float _pow) {
    simplexAmount = _amount;
    simplexDepth = _depth;
    simplexOffset = _offset;
    simplexWrap = _wrap;
    simplexPow = _pow;
    
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

void ParticleSystem::dissipate() {
    for (int i = 0; i < numParticles; i++) {
        ofVec3f position = particles[i].getPosition();
        ofVec3f destination = (position - center) * 5.0 + center;
        
        float particleSize = particles[i].getParticleSize();
        particles[i].setParticleSize(particleSize * 1.5);
        
        particles[i].setSimplexMorph(ofRandom(50, 150), 0.004, 0.01, TWO_PI, 1.0);
        particles[i].setLifetime(1.0, 0.0025);
        particles[i].setPersistence(false);
        particles[i].setRandomFollow(0.01);
        
        particles[i].setOrigin(ofVec3f(destination.x, destination.y, position.z));
    }
 }

