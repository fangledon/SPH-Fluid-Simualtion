#include "Camera.hpp"

Camera::Camera() {
	reset();
}

void Camera::update() {
}

void Camera::draw() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set perspective projection
    gluPerspective(fov, aspect, nearClip, farClip);

    // Place camera
    glTranslatef(0, 0, -distance);
    glRotatef(incline, 1.0f, 0.0f, 0.0f);
    glRotatef(azimuth, 0.0f, 1.0f, 0.0f);

    // Return to modelview matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void Camera::reset() {
	fov = 60.0f;
	aspect = 1.33f;
	nearClip = 0.1f;
	farClip = 100.0f;

	distance = 2.0f;
	azimuth = 0.0f;
	incline = 0.0f;
}
