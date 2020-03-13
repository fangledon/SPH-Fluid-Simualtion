//
//  Plane.cpp
//  CSE169
//
//  Created by Xindong Cai on 3/11/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane(Vector3 point, Vector3 normal) {
    this->point = point;
    this->normal = normal.normalize();
}

float Plane::collisionDepth(Vector3 position) {
    Vector3 collisionVector = this->point - position;
    return collisionVector.dot(this->normal);
}

void Plane::reset(Vector3 point, Vector3 normal) {
    this->point = point;
    this->normal = normal.normalize();
}

Plane::~Plane() {}
