#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Core.hpp"

/**
 The Camera class provides a simple means to controlling the 3D camera. It could
 be extended to support more interactive controls. Ultimately. the camera sets the
 GL projection and viewing matrices.
 */

class Camera {
private:
    // perspective controls
    float fov;      // field of View Angle
    float aspect;   // aspect Ratio
    float nearClip; // near clipping plane distance
    float farClip;  // far clipping plane distance

    // polar controls
    float distance;    // distance of the camera eye position to the origin
    float azimuth;    // rotation of the camera eye position around the Y axis
    float incline;    // angle of the camera eye position over the XZ plane
    
public:
	Camera();

	void update();
    
	void reset();
    
	void draw();

	// Setters and getters
	void setAspect(float a)		{ aspect = a; }
    
	void setDistance(float d)	{ distance = d; }
    
	void setAzimuth(float a)	{ azimuth = a; }
    
	void setIncline(float i)	{ incline = i; }

	float getDistance()			{ return distance; }
    
	float getAzimuth()			{ return azimuth; }
    
	float getIncline()			{ return incline; }
};

#endif
