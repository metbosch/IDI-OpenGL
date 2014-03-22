#include <iostream>
using namespace std;

#include <math.h>
#include <Vector>
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
#define model0_height 1
#define model1_height 1.5
const Point model0_place = {0, 0.5, 0};
const Point model1_place = {2.5, 0.75, -2.5};
const int START_HEIGHT = 600;
const int START_WIDTH  = 600;
const int NUM_EXPECTED_PARAMETERS = 2;

/** GLOBAL VARIABLES **/
double ANGLE_X, ANGLE_Y, ANGLE_Z;
double SCALE, ROTATION_FACTOR, SPHERE_RAD;
unsigned char MODE;
int LAST_MOUSE_X, LAST_MOUSE_Y;
vector <ModelLoader> models;


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
    cout << " Change camera mode [orto | presp]" << endl;
    cout << "  -> Press 'p' to change" << endl;
    cout << " Change the dist behind VRP and camera" << endl;
    cout << "  -> Press 'm' to redouce dist" << endl;
    cout << "  -> Press 'n' to increase dist" << endl;
    cout << " Print status information" << endl;
    cout << "  -> Press 's' to display" << endl;
    cout << " Set the response of mouse events on window" << endl;
    cout << "  -> Press 'r' to set camera rotation mode" << endl;
    cout << " Set the precision of rotation mode" << endl;
    cout << "  -> Press '+' to increment the rotation speed" << endl;
    cout << "  -> Press '-' to decrement the rotation speed" << endl;
    cout << " Reset the program to starting camera position" << endl;
    cout << "  -> Press 'i' to reset" << endl;
    cout << " Zoom in/out of camera optic" << endl;
    cout << "  -> Press 'o' to make zoom-out" << endl;
    cout << "  -> Press 'l' to make zoom-in" << endl;
    cout << " Press ESC to exit" << endl;
    cout << "-------------------------------------------------------" << endl;
}

/**
  * Display program status information on default output channel
  */
void status() {
    cout << endl;
    cout << "--------------------> >> STATUS << <-------------------" << endl;
    cout << " Camera mode    : " << getStrCameraMode() << endl;   
    cout << " Camera dist    : " << getCamDist() << endl;
    cout << " Camera aperture: " << getCamAperture() << endl;
    cout << " Rotation factor: " << ROTATION_FACTOR << " [Def. 15]"<< endl;
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
    glBegin(GL_QUADS);
        glColor3f (1, 0.3, 0);
        glVertex3f( 5, 0, 5);
        glVertex3f(-5, 0, 5);
        glVertex3f(-5, 0,-5);
        glVertex3f( 5, 0,-5);
    glEnd();
}

void paintWalls() {
    glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        glTranslated(2.5, 0.75, -2.5);
        glScaled(20, 7.5, 1);
        glutSolidCube(0.2);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        glTranslated(-4.9, 0.75, 0);
        glScaled(1, 7.5, 50);
        glutSolidCube(0.2);
    glPopMatrix();
}

void paintSnowMan(Point center) {
    glPushMatrix();
        glTranslated(center.x, center.y + 0.4, center.z);
        glColor3f(1.0, 1.0, 1.0);
        glutSolidSphere(0.4, 20, 20);
        glTranslated(0.0, 0.6, 0.0);
        glutSolidSphere(0.2, 20, 20);
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
        paintWalls();
        paintSnowMan({ 2.5, 0, 2.5});
        paintSnowMan({-2.5, 0, 2.5});
        paintSnowMan({-2.5, 0,-2.5});
        models[0].draw();
        models[1].draw();
        glColor4f(0.0, 0.0, 0.5, 0.1);
        glutWireSphere(SPHERE_RAD, 20, 20);
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
    updateCamera(relX, relY);
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
    }
    LAST_MOUSE_X = x;
    LAST_MOUSE_Y = y;
    glutPostRedisplay();
}

void onKeyboardPulse(unsigned char key, int x, int y) {
    float height = glutGet(GLUT_WINDOW_HEIGHT)/(double)(START_HEIGHT);
    float width  = glutGet(GLUT_WINDOW_WIDTH)/(double)(START_WIDTH);
    switch (key) {
        case 'h':   help();
                    break;
        case '+':   ROTATION_FACTOR *= 1.3;
                    break;
        case '-':   ROTATION_FACTOR /= 1.3;
                    break;
        case 's':   status();
                    break;
        case 'p':   changeCameraType();
                    glutPostRedisplay();
                    break;
        case 'i':   initCamera(SPHERE_RAD);
                    setCamDist(SPHERE_RAD + 0.5, 0.5, SPHERE_RAD*2+0.5);
                    updateCamera(height, width);
                    setCameraMatrix();
                    glutPostRedisplay();
                    break;
        case 'm':   incrementCamDist(SPHERE_RAD/15);
                    updateCamera(height, width);
                    glutPostRedisplay();
                    break;
        case 'n':   incrementCamDist(-SPHERE_RAD/15);
                    updateCamera(height, width);
                    glutPostRedisplay();
                    break;
        case 'o':   zoomOut();
                    updateCamera(height, width);
                    glutPostRedisplay();
                    break;
        case 'l':   zoomIn();
                    updateCamera(height, width);
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

Point calcVRP(Point min, Point max) {
    Point ret;
    ret.x = (max.x + min.x)*0.5;
    ret.y = (max.y + min.y)*0.5;
    ret.z = (max.z + min.z)*0.5;
    return ret;
}

double calcMinContainerBoxScene(Point &min, Point &max) {
    int num_els = 3;
    Point maxs[num_els - 1];
    Point mins[num_els - 1];
    // Get min and max points for all the elements on the scene
    models[0].getBoxPoints(min, max);
    models[2].getBoxPoints(mins[2], maxs[2]);
    mins[0].x = -5;
    mins[0].y = 0.0;
    mins[0].z = -5;
    maxs[0].x = 5;
    maxs[0].y = 0.0;
    maxs[0].z = 5;

    mins[1].x = -0.4;
    mins[1].y = -0.4;
    mins[1].z = -0.4;
    maxs[1].x = 0.4;
    maxs[1].y = 0.8;
    maxs[1].z = 0.4;
    for (int i = 0; i < num_els - 1; ++i) {
        if (mins[i].x < min.x) min.x = mins[i].x;
        if (maxs[i].x > max.x) max.x = maxs[i].x;
        if (mins[i].y < min.y) min.y = mins[i].y;
        if (maxs[i].y > max.y) max.y = maxs[i].y;
        if (mins[i].z < min.z) min.z = mins[i].z;
        if (maxs[i].z > max.z) max.z = maxs[i].z;
    }
}

double calcMinSphereRadius() {
    Point max, min;
    calcMinContainerBoxScene(min, max);
    return sqrt((max.x - min.x)*(max.x - min.x) + (max.y - min.y)*(max.y - min.y) + (max.z - min.z)*(max.z - min.z))*0.5;
}

/**
  * Initializate the Global variables of the program
  */
void initGlobalVars(const char *argv[]) {
    SCALE = 1.0;
    ROTATION_FACTOR = 15.0;
    MODE = 'r';
    models.push_back(ModelLoader(argv[1], model0_height, model0_place));
    models.push_back(ModelLoader(argv[1], model1_height, model1_place));
    models[1].setAngles(0, 90, 0);
    SPHERE_RAD = calcMinSphereRadius();
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

    // Registre de Callbacks
    glutDisplayFunc(refresh);
    glutReshapeFunc(onResize);
	glutMouseFunc(onMouseClick);
	glutMotionFunc(onMouseMotion);
    glutKeyboardFunc(onKeyboardPulse);

    // Initialization of openGL
    initGL();

    // Initialization of global variables
    initGlobalVars(argv);
    initCamera(SPHERE_RAD);
    setCamDist(SPHERE_RAD + 0.5, 0.5, SPHERE_RAD*2+0.5);
    setCameraMatrix();

    // GLUT events loop
    glutMainLoop();
    return 0;
}
