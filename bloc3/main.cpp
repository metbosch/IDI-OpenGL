#include <iostream>
using namespace std;

#include "modelFunctions.cpp"
#include "cameraFunctions.cpp"
#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include<GL/gl.h>
    #include<GL/freeglut.h>
#endif

/** TYPE DEFINITIONS **/
#ifndef __POINT_DEF__
#define __POINT_DEF__

struct Point {
    double x, y, z;
};

#endif

/** PROGRAM CONSTATNS **/
const int START_HEIGHT = 600;
const int START_WIDTH  = 600;
const int NUM_EXPECTED_PARAMETERS = 2;

/** GLOBAL VARIABLES **/
double ANGLE_X, ANGLE_Y, ANGLE_Z;
double SCALE, ROTATION_FACTOR;
unsigned char MODE;
int LAST_MOUSE_X, LAST_MOUSE_Y;


/**
  * Close the the program
  */
void close() {
	exit(1);
}

/**
  * Display help information on default output channel
  */
void help() {
    cout << endl;
    cout << "---------------> >> HELP INFORMATION << <--------------" << endl;
    cout << " Change camera mode [prespectiva | axonometrica]" << endl;
    cout << "  -> Press 'p' to change" << endl;
    cout << " Print status information" << endl;
    cout << "  -> Press 's' to display" << endl;
    cout << " Set the response of mouse events on window" << endl;
    cout << "  -> Press 'r' to set camera rotation mode" << endl;
//    cout << "  -> Press 't' to set translation mode" << endl;
    cout << " Set the precision of rotation mode" << endl;
    cout << "  -> Press '+' to increment the rotation speed" << endl;
    cout << "  -> Press '-' to decrement the rotation speed" << endl;
    cout << " Press ESC to exit" << endl;
    cout << "-------------------------------------------------------" << endl;
}

/**
  * Display program status information on default output channel
  */
void status() {
    cout << endl;
    cout << "--------------------> >> STATUS << <-------------------" << endl;
    cout << " Camera mode : " << getStrCameraMode() << endl;    
    cout << " Rotation factor: " << ROTATION_FACTOR << " [Def. 10]"<< endl;
    cout << "-------------------------------------------------------" << endl;
}

void usage() {
    cout << endl;
    cout << "Usage: file_with_model" << endl;
}

/**
  * Paint the axes of current object
  * The rotations and translations can affect to the painted axes
  */
void paintAxes() {
    glBegin(GL_LINES);
        glColor3f (1, 0, 0);
            glVertex3f(0, 0, 0);
            glVertex3f(1, 0, 0);
        glColor3f(0, 1, 0);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 1, 0);
        glColor3f(0, 0, 1);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 1);
    glEnd();
}

void paintFloor() {
    glPushMatrix();
        glTranslated(0.0, -0.4, 0.0);
        glBegin(GL_QUADS);
            glColor3f (0.545, 0.271, 0.075);
            glVertex3f( 0.75, 0, 0.75);
            glVertex3f( 0.75, 0,-0.75);
            glVertex3f(-0.75, 0,-0.75);
            glVertex3f(-0.75, 0, 0.75);
        glEnd();
    glPopMatrix();
}

void paintSnowMan() {
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();
            glRotated(90, 1, 0, 0);
            glutSolidSphere(0.4, 20, 20);
        glPopMatrix();
        glTranslated(0.0, 0.6, 0.0);
        glPushMatrix();
            glRotated(90, 1, 0, 0);
            glutSolidSphere(0.2, 50, 50);
        glPopMatrix();
        glColor3f(1.0, 0.5, 0.0);
        glTranslated(0.1, 0.0, 0.0);
        glRotated(90, 0, 1, 0);
        glutSolidCone(0.1, 0.2, 20, 20);
    glPopMatrix();
}

/* ----------------------- CALLBACKS ----------------------- */

void refresh () {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        glRotated(0, 1, 0, 0);
        glScaled(SCALE, SCALE, SCALE);
        paintFloor();
        paintSnowMan();
        paintModel();
    glPopMatrix();
    glutSwapBuffers();
}

/**
 * Callback for resize events
 * @param height New height of window
 * @param width New width of window
 */
void onResize(int height, int width) {
    double relX = (double)width/(double)(START_WIDTH);
    double relY = (double)height/(double)(START_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-relY, relY, -relX, relX, -1, 1);
    setMatrixMode();
    glViewport(0, 0, height, width);
}

void onMouseClick(int buton, int evnt, int x, int y) {
    if (evnt == GLUT_DOWN) {
        LAST_MOUSE_X = x;
        LAST_MOUSE_Y = y;
    }
}

void onMouseMotion(int x, int y) {
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	float width  = glutGet(GLUT_WINDOW_WIDTH);
    switch (MODE) {
        case 'r':   incrementEulerAngles(((double)(LAST_MOUSE_Y - y)*ROTATION_FACTOR/height),
                                         ((double)(x - LAST_MOUSE_X)*ROTATION_FACTOR/width), 0.0);
                    setCameraMatrix();
                    break;
        case 't':   SCALE = x >= width - y ? (double)((x)*2.0/height) :
                                             (double)((width - (y))*2.0/width);
                    break;
        case 'c':   TRANS_Z += (2.0*(y - LAST_MOUSE_Y))/(double)height;
                    TRANS_X += (2.0*(x - LAST_MOUSE_X))/(double)width;
                    break;
    }
    LAST_MOUSE_X = x;
    LAST_MOUSE_Y = y;
    glutPostRedisplay();
}

void onKeyboardPulse(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':   help();
                    break;
        case '+':   ROTATION_FACTOR *= 1.3;
                    break;
        case '-':   ROTATION_FACTOR /= 1.3;
                    break;
        case 's':   status();
                    break;
        case 'p':   changeCameraMode();
                    glutPostRedisplay();
                    break;
        case (char)27:  close();
                        break;
    }
    if (MODE == 'c' && key == 'c') MODE == 'r';
    else MODE = key;
}

/* -------------------- END OF CALLBACKS -------------------- */

/* ---------------------- INITIAL CALCS --------------------- */

/**
  * Initializate the Global variables of the program
  */
void initGlobalVars() {
    SCALE = 1.0;
    ROTATION_FACTOR = 10.0;
    MODE = 'r';

    INIT_MODEL_HEIGHT = 0.5;
    TRANS_X = 0.75;
    TRANS_Y = -0.4 + INIT_MODEL_HEIGHT/2.0;
    TRANS_Z = 0.75;
}

/**
  * Initializate the openGL envrionament
  */
void initGL() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
}

/* ------------------- END OF INITIAL CALCS ------------------ */

/* ----------------------- MAIN FUNCTION --------------------- */

int main(int argc, const char *argv[]) {
    // Check num of paramaters
    if (argc != NUM_EXPECTED_PARAMETERS) {
        usage();
        close();
    }

    // Initialitzation of GLUT
    glutInit(&argc, ((char **)argv));
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(START_HEIGHT, START_WIDTH);
    glutCreateWindow("IDI: Bloc 3");
    
    // Config rotations mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // Registre de Callbacks
    glutDisplayFunc(refresh);
    glutReshapeFunc(onResize);
	glutMouseFunc(onMouseClick);
	glutMotionFunc(onMouseMotion);
    glutKeyboardFunc(onKeyboardPulse);

    // Initialization of openGL
    initGL();

    // Initialization of global variables
    initCamera();
    initGlobalVars();
    loadModel(argv[1]);
    calcModelVars();
    setCameraMatrix();

    // GLUT events loop
    glutMainLoop();
    return 0;
}
