//
//  Plane.hpp
//  CSE169
//
//  Created by Xindong Cai on 3/11/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Vector3.hpp"

/**
 The plane used to detect collisions in particle systems
*/
class Plane {
public:
    Vector3 point;  // point on the plane
    Vector3 normal; // the normal to the plane
    
    Plane(Vector3 point, Vector3 normal);
    
    float collisionDepth(Vector3 position);
    
    void reset(Vector3 point, Vector3 normal);
    
    ~Plane();
};

#endif /* Plane_hpp */
