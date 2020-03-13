//
//  FluidParticle.hpp
//  CSE169
//
//  Created by Xindong Cai on 3/8/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef FluidParticle_hpp
#define FluidParticle_hpp

#include <stdio.h>
#include "Vector3.hpp"
#include <vector>

using namespace std;

/**
  The particle used to simulate fluid. SPH requires each particle has a list of neighbors within support radius
 */
class FluidParticle {
public:
    
    float m;            // mass
    Vector3 p;          // position
    Vector3 prevPos;    // prev position
    Vector3 v;          // velocity
    Vector3 f;          // force
    float density;
    float pressure;
    vector<unsigned int>* neighbors; // neighbor particles within support radius
    
    FluidParticle(float m, Vector3 p);
    
    void update(float deltaTime);
    
    void reset(float m, Vector3 p);
    
    ~FluidParticle();
    
};

#endif /* FluidParticle_hpp */

