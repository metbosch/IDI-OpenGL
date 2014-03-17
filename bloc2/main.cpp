#include <iostream>
using namespace std;
#include "model.h"
#include <vector>
#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include<GL/gl.h>
    #include<GL/freeglut.h>
#endif

/** TYPE DEFINITIONS **/
struct Point {
    double x, y, z;
};

/** GLOBAL VARIABLES **/
double ANGLE_X, ANGLE_Y, ANGLE_Z;
double SCALE, ROTATION_FACTOR;
unsigned char MODE;
int LAST_MOUSE_X, LAST_MOUSE_Y;

/** GLOBAL MODEL VARIABLE **/
Model model;
double REF_TRANS_X, REF_TRANS_Y, REF_TRANS_Z, SCALE_REF;
double TRANS_X, TRANS_Y, TRANS_Z;
double INIT_MODEL_HEIGHT;

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
    cout << " Set the response of mouse events on window" << endl;
    cout << "  -> Press 'r' to set rotation mode" << endl;
    cout << "  -> Press 't' to set translation mode" << endl;
    cout << " Set the precision of rotation mode" << endl;
    cout << "  -> Press '+' to increment the rotation speed" << endl;
    cout << "  -> Press '-' to decrement the rotation speed" << endl;
    cout << " Print status information" << endl;
    cout << "  -> Press 's' to display" << endl;
    cout << " Press ESC to exit" << endl;
    cout << "-------------------------------------------------------" << endl;
}

/**
  * Display program status information on default output channel
  */
void status() {
    cout << endl;
    cout << "--------------------> >> STATUS << <-------------------" << endl;
    cout << " Rotation factor: " << ROTATION_FACTOR << " [Def. 10]"<< endl;
    cout << "-------------------------------------------------------" << endl;
}

void usage() {
    cout << endl;
    cout << "Usage: file_with_model" << endl;
}

int min (int a, int b) {
    if (a > b) return b;
    return a;
}

double max (double a, double b) {
    if (a < b) return b;
    return a;
}

double max (double a, double b, double c) {
    if (max(a, b) > max(b, c)) return a;
    else if (max(a, b) < max(b, c)) return c;
    else return b;
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

/**
  * Load the model contained in a file
  * Param file: String with the name of continer file of model
  */
void loadModel(string file) {
    model = Model();
    model.load(file);
}

/**
  * Paint the model
  */
void paintModel() {
    glPushMatrix();
        glTranslated(TRANS_X, TRANS_Y, TRANS_Z);
        glScaled(SCALE_REF, SCALE_REF, SCALE_REF);
        glTranslated(REF_TRANS_X, REF_TRANS_Y, REF_TRANS_Z);
        vector<Face> triangles = model.faces();
        vector<Vertex> vertex = model.vertices();
        for (int i = 0; i < triangles.size(); ++i) {
        glBegin(GL_TRIANGLES);
        for (int j = 0; j < 3; ++j) {
            int mat = triangles[i].mat;
            if (mat >= 0 && mat < Materials.size()) {
                const float* colors = Materials[mat].diffuse;
                glColor3f(*colors, *(colors + 1), *(colors + 2));
            }

            int num = triangles[i].v[j];
            glVertex3f(vertex[num], vertex[num + 1], vertex[num + 2]);
        }
        glEnd();
    }
    glPopMatrix();
}

/**
  * Get the two vertex of minimum container box od model
  */
void getExtremsModelVertex(Point &min, Point &max) {
    vector<Vertex> vertex = model.vertices();
    if (vertex.size() > 2) {
        min.x = max.x = vertex[0];
        min.y = max.y = vertex[1];
        min.z = max.z = vertex[2];
    }
    for (int i = 3; i < vertex.size(); i += 3) {
        if (min.x > vertex[i]) min.x = vertex[i];
        else if (max.x < vertex[i]) max.x = vertex[i];

        if (min.y > vertex[i + 1]) min.y = vertex[i + 1];
        else if (max.y < vertex[i + 1]) max.y = vertex[i + 1];

        if (min.z > vertex[i + 2]) min.z = vertex[i + 2];
        else if (max.z < vertex[i + 2]) max.z = vertex[i + 2];
    }
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

void refresh () {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
     //   paintAxes();
        glRotated(ANGLE_X, 1, 0, 0);
        glRotated(ANGLE_Y, 0, 1, 0);
        glScaled(SCALE, SCALE, SCALE);
        paintFloor();
        paintSnowMan();
        paintModel();
    glPopMatrix();
    glutSwapBuffers();
}

void onResize(int height, int width) {
    int small = min(height, width);
    glViewport(max(0, (height - width)/2), max(0, (width - height)/2), small, small);
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
        case 'r':   ANGLE_X += (double)((y - LAST_MOUSE_Y)*ROTATION_FACTOR/height);
                    ANGLE_Y += (double)((x - LAST_MOUSE_X)*ROTATION_FACTOR/width);
                    break;
        case 't':   SCALE = x >= width - y ? (double)((x)*2.0/height) :
                                             (double)((width - (y))*2.0/width);
                    break;
        case 'c':   TRANS_Z += (double)((y - LAST_MOUSE_Y)/height);
                    TRANS_X += (double)((x - LAST_MOUSE_X)/width);
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
        case (char)27:  close();
                        break;
    }
    if (MODE == 'c') MODE == 'r';
    else MODE = key;
}

/**
 * Calculates the initial scale and translation for model
 */
void calcModelVars() {
    Point model_min, model_max;
    getExtremsModelVertex(model_min, model_max);

    SCALE_REF = INIT_MODEL_HEIGHT/max(model_max.x - model_min.x, model_max.y - model_min.y,
                    model_max.z - model_min.z);
    REF_TRANS_X = -(model_min.x + model_max.x)/2.0;
    REF_TRANS_Y = -(model_min.y + model_max.y)/2.0;
    REF_TRANS_Z = -(model_min.z + model_max.z)/2.0;
}

/**
  * Initializate the Global variables of the program
  */
void initGlobalVars() {
    ANGLE_Y = ANGLE_Z = ANGLE_X = 0.0;
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

int main(int argc, const char *argv[]) {
    // Check num of paramaters
    if (argc != 2) {
        usage();
        close();
    }

    // Initialitzation of GLUT
    glutInit(&argc, ((char **)argv));
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practica 1");
    
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
    initGlobalVars();
    loadModel(argv[1]);
    calcModelVars();

    // GLUT events loop
    glutMainLoop();
    return 0;
}
