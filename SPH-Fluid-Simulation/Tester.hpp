#ifndef TESTER_HPP
#define TESTER_HPP

#include "Core.hpp"
#include "Camera.hpp"
#include "Fluid.hpp"

/**
The Tester is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

class Tester {
private:
    
    // window management
    int windowHandle;
    int winX,winY;

    // input
    bool leftDown, middleDown, rightDown;
    int mouseX, mouseY;

    // components
    Camera * cam;
    Fluid * fluid;
    
public:
	Tester(int argc,char **argv);

	void update();
    
    void draw();
    
	void reset();

	void quit();
    
    ~Tester();

	// Event handlers
	void resize(int x, int y);
    
	void keyboard(int key, int x, int y);
    
	void mouseButton(int btn, int state, int x, int y);
    
	void mouseMotion(int x, int y);
    
};

#endif
