//
//  SpatialHashTable.hpp
//  CSE169
//
//  Created by Xindong Cai on 3/10/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef SpatialHashTable_hpp
#define SpatialHashTable_hpp

#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <list>
#include "FluidParticle.hpp"

/**
 A spatial hash table used for neighbors searching in 3D space. Each entry of the hash table is a doubly linked list.
 For each particle in 3D space, the key is its position in 3D space, and the value is the id of that particle.
 Each 3D grid cell has size : (cellSpace * cellSpace * cellSpace), and all particles in that grid cell will be hashed to
 same index in the table.
 To find neighbors within radius = cellSpace of a given particle, query for the nearby 27 cells (3 * 3 cube).
 This works the same as uniform grids but saves lots of memory, as the memory is proportional to the number of used grids.
 */
class SpatialHashTable {
private:
    vector<list<unsigned int>*> table;
    
    unsigned int tableSize;
    
    float cellSpace;
    
    // large prime numbders used for hashing
    unsigned int p1 = 73856093;
    unsigned int p2 = 19349663;
    unsigned int p3 = 83492791;
    
    unsigned int hashVal(float x, float y, float z);
    
public:
    
    SpatialHashTable();

    SpatialHashTable(unsigned int tableSize, float cellSpace);
    
    void insert(Vector3& p, unsigned int id);
        
    void findNeighbors(vector<FluidParticle*>& particles, unsigned int id);
    
    void update(FluidParticle* particle, unsigned int id);

    void remove(Vector3& p, unsigned int id);
    
    void clear();
    
    ~SpatialHashTable();
    
};

#endif /* SpatialHashTable_hpp */
