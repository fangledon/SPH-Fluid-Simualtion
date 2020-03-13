//
//  Fluid.hpp
//  CSE169
//
//  Created by Xindong Cai on 3/8/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Fluid_hpp
#define Fluid_hpp

#include <stdio.h>
#include <vector>
#include "Core.hpp"
#include "SpatialHashTable.hpp"
#include "Plane.hpp"

// constants used in SPH simulation

#define NUM_X 15 // 15 // 10
#define NUM_Y 40 // 40 // 10
#define NUM_Z 2 // 2 // 10
#define REST_DENSITY 1000.0f
#define MASS 0.02f
#define SUPPORT_RADIUS 0.045f
#define STIFFNESS 3.0f
#define VISCOSITY 0.0015f
#define EPSILON 0.0001f
#define NUM_SAMPLE 10 // 5
#define DELTA_TIME 1.0f / 600.0f // 1/300
#define HALF_BOX_X 0.30f // 0.30 // 0.15
#define HALF_BOX_Y 0.70f // 0.70 // 0.15
#define HALF_BOX_Z 0.025f // 0.025 // 0.15
#define SPRING_COLLISION true
#define PLANE_SPRING_CONST 15000.0f
#define PLANE_DAMPING_CONST 1.0f
#define E 0.5
#define MOVE_SPEED 0.01f

#define PI 3.1415926f

using namespace std;

class Fluid {
private:
    
    float h;
    float h3;
    float particleRadius;
    
    // the boundary of the fluid container
    float boxXmin = -HALF_BOX_X;
    float boxXmax = HALF_BOX_X;
    float boxYmin = -HALF_BOX_Y;
    float boxYmax = HALF_BOX_Y;
    float boxZmin = -HALF_BOX_Z;
    float boxZmax = HALF_BOX_Z;
    
    SpatialHashTable * table;
    vector<FluidParticle*> particles;
    vector<Plane*> planes;
    
    void initParticles();
    
    void initPlanes();
    
    void resetParticles();
    
    void resetPlanes();
    
    float kernel(FluidParticle* p1, FluidParticle* p2);
    
    Vector3 gradKernel(FluidParticle* p1, FluidParticle* p2);

    float computeDensity(FluidParticle* particle);
    
    float computePressure(FluidParticle* particle);
    
    Vector3 computePressureForce(FluidParticle* particle);
    
    Vector3 computeViscocityForce(FluidParticle* particle);
    
    Vector3 computeOtherForce(FluidParticle* particle);
    
    void handleCollision(FluidParticle* particle, int id);

public:
    
    Fluid();
    
    void draw();
    
    void update();
    
    void reset();
    
    void moveBoxLeft();
    
    void moveBoxRight();
    
    ~Fluid();
    
};

#endif /* Fluid_hpp */
