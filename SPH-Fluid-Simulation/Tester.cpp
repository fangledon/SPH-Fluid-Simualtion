#include "Tester.hpp"
#include <string>

#define WINDOWTITLE	"SPH Fluid Simulation"

using namespace std;

static Tester* TESTER;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc, argv);
	glutMainLoop();
	return 0;
}

// make glut call member functions instead of static functions
static void Display() {
    TESTER->draw();
}

static void Idle() {
    TESTER->update();
}

static void Resize(int x,int y) {
    TESTER->resize(x,y);
}

static void Keyboard(unsigned char key,int x,int y)	{
    TESTER->keyboard(key,x,y);
}

static void Mousebutton(int btn,int state,int x,int y)	{
    TESTER->mouseButton(btn, state, x, y);
}

static void Mousemotion(int x, int y){
    TESTER->mouseMotion(x,y);
}

Tester::Tester(int argc,char **argv) {
	winX = 800;
	winY = 800;
	leftDown = middleDown = rightDown = false;
	mouseX = mouseY = 0;

	// create the window
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(0, 0);
	windowHandle = glutCreateWindow(WINDOWTITLE);
    
	glutSetWindowTitle(WINDOWTITLE);
	glutSetWindow(windowHandle);

	// background color
	glClearColor( 1., 1., 1., 1. );
    
	// callbacks
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mousebutton);
	glutMotionFunc(Mousemotion);
	glutPassiveMotionFunc( Mousemotion );
	glutReshapeFunc(Resize);
    glEnable(GL_CULL_FACE);

    cam = new Camera();
	cam->setAspect(float(winX) / float(winY));
    
    fluid = new Fluid();
    
    // GL lighting
    
    GLfloat lightPos0[] = { 0.0, 1.0, 1.0, 0.0 };
    GLfloat spotDir0[] = { 0.0, -1.0, -1.0 };
    GLfloat ambient0[] = { 0.0, 1.0, 1.0, 0.0 };
    GLfloat diffuse0[] = { 0.5, 0.5, 0.5, 0.5};
    GLfloat specular0[] = { 0.5, 0.5, 0.5, 0.5 };
    
    GLfloat lightPos1[] = { 1.0, 0.0, -1.0, 0.0 };
    GLfloat spotDir1[] = { -1.0, 0.0, 1.0 };
    GLfloat ambient1[] = { 0.1, 0.1, 0.9, 0.1};
    GLfloat diffuse1[] = { 0.5, 0.5, 0.5, 0.5 };
    GLfloat specular1[] = { 0.5, 0.5, 0.5, 0.5 };
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
        
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
        
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    
}

void Tester::update() {
	// update the components in the world
	cam->update();
    fluid->update();

    glClearColor( 1., 1., 1., 1. );
    
	// tell glut to re-display the scene
	glutSetWindow(windowHandle);
	glutPostRedisplay();
}

void Tester::draw() {

	// begin drawing scene
	glViewport(0, 0, winX, winY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw components
	cam->draw();		// Sets up projection & viewing matrices
    fluid->draw();

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

void Tester::reset() {
    fluid->reset();
    cam->reset();
    cam->setAspect(float(winX) / float(winY));
}

void Tester::quit() {
	glFinish();
	glutDestroyWindow(windowHandle);
	exit(0);
}

void Tester::resize(int x, int y) {
	winX = x;
	winY = y;
	cam->setAspect(float(winX) / float(winY));
}

void Tester::keyboard(int key, int x, int y) {
    switch(key) {
        case 'a': { // move the fluid containter to left
            fluid->moveBoxLeft();
            break;
        }
        case 'd': { // move the fluid containter to right
            fluid->moveBoxRight();
            break;
        }
        case 'r': {
            reset();
            break;
        }
    }
}

void Tester::mouseButton(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON) {
		leftDown = (state == GLUT_DOWN);
	}
	else if (btn == GLUT_MIDDLE_BUTTON) {
		middleDown = (state == GLUT_DOWN);
	}
	else if (btn == GLUT_RIGHT_BUTTON) {
		rightDown = (state == GLUT_DOWN);
	}
}

void Tester::mouseMotion(int nx, int ny) {
	int dx = nx - mouseX;
	int dy = -(ny - mouseY);

	mouseX = nx;
	mouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(leftDown) {
		const float rate = 1.0f;
		cam->setAzimuth(cam->getAzimuth() + dx * rate);
		cam->setIncline(cam->getIncline()- dy * rate);
	}
	if(rightDown) {
		const float rate = 0.01f;
		cam->setDistance(cam->getDistance() * (1.0f - dx * rate));
	}
}

Tester::~Tester() {
    delete cam;
    delete fluid;
    glFinish();
    glutDestroyWindow(windowHandle);
}
