//
//  Fluid.cpp
//  CSE169
//
//  Created by Xindong Cai on 3/8/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Fluid.hpp"

Fluid::Fluid() {
    h = SUPPORT_RADIUS / 2.0f;
    h3 = powf(h, 3.0f);
    unsigned int numParticles = NUM_X * NUM_Y * NUM_Z;
    table = new SpatialHashTable(numParticles, SUPPORT_RADIUS);
    initParticles();
    initPlanes();
}

void Fluid::initParticles() {
    // initialize a cuboid of fluid particles next to each other
    
    float particleVolume = MASS / REST_DENSITY;
    particleRadius = powf((particleVolume * 3.0f) / (4.0 * PI), 1.0f / 3.0f);
    float diameter = particleRadius * 2.0f;
    
    float xStart = -(NUM_X - 1) * particleRadius;
    float yStart = -(NUM_Y - 1) * particleRadius;
    float zStart = -(NUM_Z - 1) * particleRadius;
    float xEnd = -xStart, yEnd = -yStart, zEnd = -zStart;

    unsigned int id = 0;
    for (float x = xStart; x <= xEnd; x += diameter) {
        for (float y = yStart; y <= yEnd; y += diameter) {
            for (float z = zStart; z <= zEnd; z += diameter) {
                FluidParticle* particle = new FluidParticle(MASS, Vector3(x, y, z));
                particles.push_back(particle);
                table->insert(particle->p, id);
                id++;
            }
        }
    }
}

void Fluid::initPlanes() {
    // push 6 planes that defines the box
    
    // top and bottom planes
    planes.push_back(new Plane(Vector3(0.0f, boxYmax, 0.0f), Vector3(0.0f, -1.0f, 0.0f)));
    planes.push_back(new Plane(Vector3(0.0f, boxYmin, 0.0f), Vector3(0.0f, 1.0f, 0.0f)));
    
    // front and back planes
    planes.push_back(new Plane(Vector3(0.0f, 0.0f, boxZmax), Vector3(0.0f, 0.0f, -1.0f)));
    planes.push_back(new Plane(Vector3(0.0f, 0.0f, boxZmin), Vector3(0.0f, 0.0f, 1.0f)));
    
    // right and left planes
    planes.push_back(new Plane(Vector3(boxXmax, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));
    planes.push_back(new Plane(Vector3(boxXmin, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)));
}

void Fluid::resetParticles() {
    float diameter = particleRadius * 2.0f;
    
    float xStart = -(NUM_X - 1) * particleRadius;
    float yStart = -(NUM_Y - 1) * particleRadius;
    float zStart = -(NUM_Z - 1) * particleRadius;
    float xEnd = -xStart, yEnd = -yStart, zEnd = -zStart;

    unsigned int id = 0;
    for (float x = xStart; x <= xEnd; x += diameter) {
        for (float y = yStart; y <= yEnd; y += diameter) {
            for (float z = zStart; z <= zEnd; z += diameter) {
                particles[id]->reset(MASS, Vector3(x, y, z));
                table->insert(particles[id]->p, id);
                id++;
            }
        }
    }
}

void Fluid::resetPlanes() {
    boxXmin = -HALF_BOX_X; boxXmax = HALF_BOX_X;
    boxYmin = -HALF_BOX_Y; boxYmax = HALF_BOX_Y;
    boxZmin = -HALF_BOX_Z; boxZmax = HALF_BOX_Z;
    
    planes[0]->reset(Vector3(0.0f, boxYmax, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
    planes[1]->reset(Vector3(0.0f, boxYmin, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
    
    planes[2]->reset(Vector3(0.0f, 0.0f, boxZmax), Vector3(0.0f, 0.0f, -1.0f));
    planes[3]->reset(Vector3(0.0f, 0.0f, boxZmin), Vector3(0.0f, 0.0f, 1.0f));
    
    planes[4]->reset(Vector3(boxXmax, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
    planes[5]->reset(Vector3(boxXmin, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
}

void Fluid::draw() {
    for (FluidParticle* particle : particles) {
        glPushMatrix();
        glTranslatef(particle->p.x, particle->p.y, particle->p.z);
        glutSolidSphere(particleRadius, 10, 10);
        glPopMatrix();
    }
    
    // expand each plane by the radius to get the correct box
    float xMin = boxXmin - particleRadius, xMax = boxXmax + particleRadius;
    float yMin = boxYmin - particleRadius, yMax = boxYmax + particleRadius;
    float zMin = boxZmin - particleRadius, zMax = boxZmax + particleRadius;
    
    glDisable(GL_LIGHTING);
    drawWireBox(xMin, yMin, zMin, xMax, yMax, zMax);
    glEnable(GL_LIGHTING);
}

void Fluid::update() {
    // oversampling to improve stability
    for (int i = 0; i < NUM_SAMPLE; i++) {
        // find all neighbors of each particle
        for (int j = 0; j < particles.size(); j++) {
            table->findNeighbors(particles, j);
        }
        
        // compute density and pressure
        for (int j = 0; j < particles.size(); j++) {
            particles[j]->density = computeDensity(particles[j]);
            particles[j]->pressure = computePressure(particles[j]);
        }
        
        // compute all forces and apply to each particle
        for (int j = 0; j < particles.size(); j++) {
            Vector3 pressureForce = computePressureForce(particles[j]);
            Vector3 viscocityForce = computeViscocityForce(particles[j]);
            Vector3 otherForce = computeOtherForce(particles[j]);
            particles[j]->f = pressureForce + viscocityForce + otherForce;
        }
        
        // apply force to each particle and update spatial table
        for (int j = 0; j < particles.size(); j++) {
            handleCollision(particles[j], j);
            particles[j]->update(DELTA_TIME);
            table->update(particles[j], j);
        }
    }
    
}

float Fluid::computeDensity(FluidParticle* particle) {
    float density = 0.0f;
    for (unsigned int neighborId : *(particle->neighbors)) {
        density += kernel(particle, particles[neighborId]);
    }
    return MASS * density;
}

float Fluid::computePressure(FluidParticle* particle) {
    return STIFFNESS * (powf(particle->density / REST_DENSITY, 7) - 1);
}

Vector3 Fluid::computePressureForce(FluidParticle* particle) {
    Vector3 force = Vector3();
    float a = particle->pressure / (particle->density * particle->density);
    for (unsigned int neighborId : *(particle->neighbors)) {
        float b = particles[neighborId]->pressure / (particles[neighborId]->density * particles[neighborId]->density);
        force += particles[neighborId]->m * (a + b) * gradKernel(particle, particles[neighborId]);
    }
    force = -particle->m * force;
    return force;
}

Vector3 Fluid::computeViscocityForce(FluidParticle* particle) {
    Vector3 force = Vector3();
    for (unsigned int neighborId : *(particle->neighbors)) {
        FluidParticle* neighbor = particles[neighborId];
        Vector3 vij = particle->v - neighbor->v;
        Vector3 xij = particle->p - neighbor->p;
        force += vij * (neighbor->m / neighbor->density) * (xij.dot(gradKernel(particle, neighbor)) / (xij.dot(xij) + 0.01 * h * h));
    }
    force = particle->m * VISCOSITY * 2.0f * force;
    return force;
}

Vector3 Fluid::computeOtherForce(FluidParticle* particle) {
    Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);
    return particle->m * gravity;
}

float Fluid::kernel(FluidParticle* p1, FluidParticle* p2) {
    float q = p1->p.dist(p2->p) / h;
    float q2 = q * q;
    float constant = 3.0f / (2.0f * PI);
    
    if (q < 1.0f) { // 0 <= q < 1
        return constant * (2.0f / 3.0f - q2 + 0.5f * q2 * q) / h3;
    }
    else { // 1 <= q < 2
        return constant * (1.0f / 6.0f) * powf(2.0f - q, 3.0f) / h3;
    }

}

Vector3 Fluid::gradKernel(FluidParticle* p1, FluidParticle* p2) {
    float dist = p1->p.dist(p2->p);
    float q = dist / h;
    
    float constant = 0.0f;
    if (q < 1.0f) { // 0 <= q < 1
        constant = 1.5f * (-2.0f * q + 1.5f * q * q) / (PI * h3 * h * dist);
    }
    else { // 1 <= q < 2
        constant = -0.75f * powf(2.0f - q, 2.0f) / (PI * h3 * h * dist);
    }
    float x = constant * (p1->p.x - p2->p.x);
    float y = constant * (p1->p.y - p2->p.y);
    float z = constant * (p1->p.z - p2->p.z);
    return Vector3(x, y, z);
}

void Fluid::handleCollision(FluidParticle* particle, int id) {
    float x = particle->p.x;
    float y = particle->p.y;
    float z = particle->p.z;
    
    if (x <= boxXmax && x >= boxXmin && y <= boxYmax && y >= boxYmin && z <= boxZmax && z >= boxZmin) {
        return;
    }
    
    // the plane is treated as a stiff spring to handle collision. It gives
    // better results near the boundary, but particles can go off the boundary a little)
    if (SPRING_COLLISION) {
        for (int i = 0; i < planes.size(); i++) {
            float depth = planes[i]->collisionDepth(Vector3(x, y, z));
            if (depth > 0.0f) {
                    particle->f += particle->m * (PLANE_SPRING_CONST * depth * planes[i]->normal);
                    particle->f += particle->m * (PLANE_DAMPING_CONST * particle->v.dot(planes[i]->normal) * planes[i]->normal);
            }
        }
    }
    else { // use normal particle and plane collision
        // particles tend to stick to the boundary, but seems to have slightly better dropping effects
        
        // find the planes with max collision depth and handle collision at that plane
        int maxIdx = 0;
        float maxDepth = 0.0f;
        for (int i = 0; i < planes.size(); i++) {
            float depth = planes[i]->collisionDepth(Vector3(x, y, z)) + EPSILON; // add small offset
            if (depth > maxDepth) {
                maxIdx = i;
                maxDepth = depth;
            }
        }
        
        // update particle position and velocity
        particle->v -= (1 + E) * (particle->v.dot(planes[maxIdx]->normal)) * planes[maxIdx]->normal;
        particle->prevPos = particle->p;
        particle->p += maxDepth * planes[maxIdx]->normal;
        table->update(particle, id);
    }
}

void Fluid::reset() {
    table->clear();
    resetParticles();
    resetPlanes();
}

void Fluid::moveBoxLeft() {
    boxXmax -= MOVE_SPEED;
    boxXmin -= MOVE_SPEED;
    planes[4]->point.x = boxXmax;
    planes[5]->point.x = boxXmin;
}

void Fluid::moveBoxRight() {
    boxXmax += MOVE_SPEED;
    boxXmin += MOVE_SPEED;
    planes[4]->point.x = boxXmax;
    planes[5]->point.x = boxXmin;
}

Fluid::~Fluid() {
    for (FluidParticle* particle : particles) {
        delete particle;
    }
    for (Plane* plane : planes) {
        delete plane;
    }
    delete table;
}
