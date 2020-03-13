//
//  FluidParticle.cpp
//  CSE169
//
//  Created by Xindong Cai on 3/8/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "FluidParticle.hpp"

FluidParticle::FluidParticle(float m, Vector3 p) {
    this->m = m;
    this->p = p;
    prevPos = p;
    v = Vector3();
    f = Vector3();
    density = 0.0f;
    pressure = 0.0f;
    neighbors = new vector<unsigned int>();
}

void FluidParticle::update(float deltaTime) {
    prevPos = p;
    
    // use forward euler integration
    v += deltaTime * (f / m);
    p += deltaTime * v;
    f.zero();
}

void FluidParticle::reset(float m, Vector3 p) {
    this->m = m;
    this->p = p;
    prevPos = p;
    v = Vector3();
    f = Vector3();
    density = 0.0f;
    pressure = 0.0f;
    if (neighbors != nullptr) delete neighbors;
    neighbors = new vector<unsigned int>();
}

FluidParticle::~FluidParticle() {
    delete neighbors;
}
