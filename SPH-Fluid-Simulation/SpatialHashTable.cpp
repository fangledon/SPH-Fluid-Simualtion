//
//  SpatialHashTable.cpp
//  CSE169
//
//  Created by Xindong Cai on 3/10/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "SpatialHashTable.hpp"

SpatialHashTable::SpatialHashTable() {}

SpatialHashTable::SpatialHashTable(unsigned int tableSize, float cellSpace) {
    this->cellSpace = cellSpace;
    this->tableSize = tableSize;
    table = vector<list<unsigned int>*>(tableSize);
}

unsigned int SpatialHashTable::hashVal(float x, float y, float z) {
    int h1 = floor(x / cellSpace);
    int h2 = floor(y / cellSpace);
    int h3 = floor(z / cellSpace);
                   
    int hash = (h1 * p1) ^ (h2 * p2) ^ (h3 * p3);
    return abs(hash) % tableSize;
}
    
void SpatialHashTable::insert(Vector3& p, unsigned int id) {
    unsigned int hash = hashVal(p.x, p.y, p.z);
    if (table[hash] == nullptr) table[hash] = new list<unsigned int>();
    table[hash]->push_back(id);
}

void SpatialHashTable::findNeighbors(vector<FluidParticle*>& particles, unsigned int id) {
    
    float x = particles[id]->p.x;
    float y = particles[id]->p.y;
    float z = particles[id]->p.z;
    
    particles[id]->neighbors->clear();
    
    // use to avoid adding duplicate neighbors
    unordered_set<unsigned int> usedHashVal;
    
    // check nearby 27 cells to find neighbors of particle with given id
    for (float a = -cellSpace; a <= cellSpace; a += cellSpace) {
        for (float b = -cellSpace; b <= cellSpace; b += cellSpace) {
            for (float c = -cellSpace; c <= cellSpace; c += cellSpace) {
                unsigned int hash = hashVal(x + a, y + b, z + c);
                if (table[hash] == nullptr) continue;
                
                if (usedHashVal.count(hash) > 0) continue;
                usedHashVal.insert(hash);
                
                for (unsigned int neighborId : *(table[hash])) {
                    if (id == neighborId) continue; // skip current particle
                    float dist = particles[id]->p.dist(particles[neighborId]->p);
                    if (dist < cellSpace) {
                        particles[id]->neighbors->push_back(neighborId);
                    }
                }
            }
        }
    }
}

void SpatialHashTable::update(FluidParticle* particle, unsigned int id) {

    // if the position of given particle moved to another grid cell, update the table
    
    int prevCellX = floor(particle->prevPos.x / cellSpace);
    int currCellX = floor(particle->p.x / cellSpace);
    if (prevCellX != currCellX) {
        remove(particle->prevPos, id);
        insert(particle->p, id);
        return;
    }
    
    int prevCellY = floor(particle->prevPos.y / cellSpace);
    int currCellY = floor(particle->p.y / cellSpace);
    if (prevCellY != currCellY) {
        remove(particle->prevPos, id);
        insert(particle->p, id);
        return;
    }
    
    int prevCellZ = floor(particle->prevPos.z / cellSpace);
    int currCellZ = floor(particle->p.z / cellSpace);
    if (prevCellZ != currCellZ) {
        remove(particle->prevPos, id);
        insert(particle->p, id);
        return;
    }

}

void SpatialHashTable::remove(Vector3& p, unsigned int id) {
    unsigned int hash = hashVal(p.x, p.y, p.z);
    if (table[hash] == nullptr) return;
    table[hash]->remove(id);
}

void SpatialHashTable::clear() {
    for (unsigned int i = 0; i < table.size(); i++) {
        if (table[i] != nullptr) {
            table[i]->clear();
        }
    }
}

SpatialHashTable::~SpatialHashTable() {
    for (unsigned int i = 0; i < table.size(); i++) {
        if (table[i] != nullptr) {
            delete table[i];
        }
    }
}
