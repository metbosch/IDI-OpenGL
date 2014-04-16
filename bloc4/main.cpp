#include <iostream>
using namespace std;

#include <math.h>
#include <vector>
#include "modelFunctions.cpp"
#include "cameraFunctions.cpp"
#include "auxFunctions.cpp"
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
bool draw_walls, enable_light, norm_vert;


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
	cout << " Change camera position [external | internal]" << endl;
    cout << "  -> Press 'c' to change (available options will change)" << endl;
    cout << " Change the dist behind VRP and camera" << endl;
    cout << "  -> Press 'm' to redouce dist" << endl;
    cout << "  -> Press 'k' to increase dist" << endl;
	cout << " Enable/Disable walls draw" << endl;
    cout << "  -> Press 'v' to invert current option" << endl;
    cout << " Enable/Disable lighting" << endl;
    cout << "  -> Press 'i' to invert current option" << endl;
    cout << " Enable/Disable noramal per vertex" << endl;
    cout << "  -> Press 'n' to invert current option" << endl;
    cout << " Print status information" << endl;
    cout << "  -> Press 'y' to display" << endl;
    cout << " Set the response of mouse events on window" << endl;
    cout << "  -> Press 'e' to set change euler angles mode" << endl;
    cout << " Set the precision of rotation mode" << endl;
    cout << "  -> Press '+' to increment the rotation speed" << endl;
    cout << "  -> Press '-' to decrement the rotation speed" << endl;
    cout << " Reset the program to starting camera position" << endl;
    cout << "  -> Press 'r' to reset" << endl;
    cout << " Optical Camera zoom in/out" << endl;
    cout << "  -> Press 'o' to make zoom-out" << endl;
    cout << "  -> Press 'l' to make zoom-in" << endl;
    cout << " Press ESC to exit" << endl;
    cout << "-------------------------------------------------------" << endl;
}

/**
  * Display extra help information of Tafaner Mode
  */
void helpTafaner() {
    cout << " Change camera angles relative to model ('Tafaner' mode)" << endl;
    cout << "  -> Use arrows to move the camera" << endl;
}

/**
  * Display help information on default output channel
  */
void help2() {
    cout << endl;
    cout << "---------------> >> HELP INFORMATION << <--------------" << endl;
    cout << " Change camera position [external | internal]" << endl;
    cout << "  -> Press 'c' to change (available options will change)" << endl;
    cout << " Rotates right or left model" << endl;
    cout << "  -> Press 'd' to rotate right" << endl;
    cout << "  -> Press 'a' to rotate left" << endl;
    cout << " Print status information" << endl;
    cout << "  -> Press 'y' to display" << endl;
	cout << " Enable/Disable walls draw" << endl;
    cout << "  -> Press 'v' to invert current option" << endl;    
    cout << " Enable/Disable lighting" << endl;
    cout << "  -> Press 'i' to invert current option" << endl;
    cout << " Enable/Disable noramal per vertex" << endl;
    cout << "  -> Press 'n' to invert current option" << endl;
    cout << " Set mode of internal camera [default | 'tafaner']" << endl;
    cout << "  -> Press 't' to change" << endl;
	if (MODE == 't') helpTafaner();
    cout << " Set velocity of camera moves" << endl;
    cout << "  -> Press 'z' to increment velocity" << endl;
    cout << "  -> Press 'x' to decrement velocity" << endl;
    cout << " Move camera arround the scene" << endl;
    cout << "  -> Press 'w' to move front" << endl;
    cout << "  -> Press 's' to move back" << endl;
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

/**
  * Display program status information on default output channel
  */
void status2() {
    cout << endl;
    cout << "--------------------> >> STATUS << <-------------------" << endl;
    cout << " Camera mode    : " << ((MODE == 'c') ? "Internal" : "Internal 'Tafaner'") << endl;   
    cout << " 'Tafaner' alfa : " << getAlfa() << endl;
	cout << " 'Tafaner' beta : " << getBeta() << endl;
    cout << " Camera aperture: " << "60º" << endl;
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
    glColor3f (0.0, 0.0, 1.0);
    float vec1[] = {0.0, 0.0, 1.0, 0.1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vec1);
    float vec2[] = {0.0, 0.0, 1.0, 0.6};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vec2);
    float vec3[] = {0.0, 0.0, 1.0, 0.9};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vec3);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40.0);
    glNormal3f(0, 1, 0);
    glBegin(GL_QUADS);
        glVertex3f( 5, 0, 5);
        glVertex3f(-5, 0, 5);
        glVertex3f(-5, 0,-5);
        glVertex3f( 5, 0,-5);
    glEnd();
}

void paintWalls() {
    glColor3f(0.0, 1.0, 0.0);
    float vec1[] = {0.0, 1.0, 0.0, 0.1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vec1);
    float vec2[] = {0.0, 1.0, 0.0, 0.6};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vec2);
    float vec3[] = {0.0, 1.0, 0.0, 0.9};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vec3);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
    glPushMatrix();
        glTranslated(2.5, 0.75, -1.5);
        glScaled(20, 7.5, 1);
        glutSolidCube(0.2);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-4.9, 0.75, 0);
        glScaled(1, 7.5, 50);
        glutSolidCube(0.2);
    glPopMatrix();
}

void paintSnowMan(Point center) {
    glPushMatrix();
        glTranslated(center.x, center.y + 0.4, center.z);
        glColor3f(1.0, 1.0, 1.0);
        float vec1[] = {0.9, 0.9, 0.9, 0.1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vec1);
        float vec2[] = {0.9, 0.9, 0.9, 0.6};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vec2);
        float vec3[] = {0.9, 0.9, 0.9, 0.9};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vec3);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
        glutSolidSphere(0.4, 20, 20);
        glTranslated(0.0, 0.6, 0.0);
        glutSolidSphere(0.2, 20, 20);
        glColor3f(1.0, 0.5, 0.0);
        float vec4[] = {1.0, 0.5, 0.0, 0.1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vec4);
        float vec5[] = {1.0, 0.5, 0.0, 0.6};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vec5);
        float vec6[] = {1.0, 0.5, 0.0, 0.9};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vec6);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
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
        if (draw_walls) paintWalls();
        paintSnowMan({ 2.5, 0, 2.5});
        paintSnowMan({-2.5, 0, 2.5});
        paintSnowMan({-2.5, 0,-2.5});
        models[0].draw(norm_vert);
        models[1].draw(norm_vert);
        glColor4f(0.0, 0.0, 0.5, 0.1);
      //glutWireSphere(SPHERE_RAD, 60, 60);
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
	switch (MODE) {
		case 'c':	initPersonalCam(relX, relY);
					break;
		case 't':	initPersonalCam(relX, relY);
					break;
		default:	updateCamera(relX, relY);
					break;
	}    
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
        case 'e':   incrementEulerAngles(((double)(LAST_MOUSE_Y - y)*ROTATION_FACTOR/height),
                                         ((double)(x - LAST_MOUSE_X)*ROTATION_FACTOR/width), 0.0);
                    setCameraMatrix();
                    break;
    }
	LAST_MOUSE_X = x;
    LAST_MOUSE_Y = y;
    glutPostRedisplay();
}

void onKeyboardPulse(unsigned char key, int x, int y) {
    float height = glutGet(GLUT_WINDOW_HEIGHT)/(double)(START_HEIGHT);
    float width  = glutGet(GLUT_WINDOW_WIDTH)/(double)(START_WIDTH);

    if (MODE == 'c' || MODE == 't') {
        switch (key) {
            case 'a':   models[0].increaseAngles(0, 3, 0);
                        updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
                        glutPostRedisplay();
                        break;
            case 'c':   updateCamera(height, width);
                        setCameraMatrix();
                        glutPostRedisplay();
                        MODE = 'e';
                        break;
            case 'd':   models[0].increaseAngles(0,-3, 0);
                        updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
                        glutPostRedisplay();
                        break;
            case 'h':   help2();
                        break;
            case 'i':   enable_light = !enable_light;
                        if (enable_light) glEnable(GL_LIGHTING);
                        else glDisable(GL_LIGHTING);
                        glutPostRedisplay();
                        break;
            case 's':   models[0].walk(-velocity);
                        updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
                        glutPostRedisplay();
                        break;
			case 'v':	draw_walls = !draw_walls;
						glutPostRedisplay();
						break;
            case 'w':   models[0].walk(velocity);
                        updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
                        glutPostRedisplay();
                        break;
            case 'x':   decreaseVelocity();
                        break;
            case 'y':   status2();
                        break;
            case 'z':   increaseVelocity();
                        break;
            case (char)27:  close();
                            break;
        }
		if (MODE == 't' && key == 't') {
			MODE = 'c';
			resetAlfaBeta();
			updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
			glutPostRedisplay();
		} else if (key == 't'){
			MODE = 't';
		}
    }
    else {
        MODE = key;
        switch (key) {
            case 'h':   help();
                        break;
            case 'i':   enable_light = !enable_light;
                        if (enable_light) glEnable(GL_LIGHTING);
                        else glDisable(GL_LIGHTING);
                        glutPostRedisplay();
                        break;
            case '+':   ROTATION_FACTOR *= 1.3;
                        break;
            case '-':   ROTATION_FACTOR /= 1.3;
                        break;
            case 'y':   status();
                        break;
            case 'p':   changeCameraType();
                        glutPostRedisplay();
                        break;
            case 'r':   initCamera(SPHERE_RAD);
                        setCamDist(SPHERE_RAD + 0.5, 0.5, SPHERE_RAD*2+0.5);
                        updateCamera(height, width);
                        setCameraMatrix();
                        glutPostRedisplay();
                        break;
            case 'm':   incrementCamDist(SPHERE_RAD/15);
                        updateCamera(height, width);
                        glutPostRedisplay();
                        break;
            case 'n':   norm_vert = !norm_vert;
                        glutPostRedisplay();
                        break;
            case 'k':   incrementCamDist(-SPHERE_RAD/15);
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
            case 'c':   initPersonalCam(height, width);
				        updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
						glutPostRedisplay();
                        MODE = 'c';
                        break;
			case 'v':	draw_walls = !draw_walls;
						glutPostRedisplay();
						break;
            case (char)27:  close();
                            break;
        }
    }
}

void onKeyboardSpecialPulse(int key, int x, int y) {
	if (MODE == 't') {
		switch(key) {
			case GLUT_KEY_UP:	increaseBeta();
								updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
								glutPostRedisplay();
								break;
			case GLUT_KEY_DOWN:	decreaseBeta();
								updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
								glutPostRedisplay();
								break;
			case GLUT_KEY_RIGHT:	decreaseAlfa();
									updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
									glutPostRedisplay();
									break;
			case GLUT_KEY_LEFT:	increaseAlfa();
								updatePersonalCam(models[0].getPosition(), models[0].getAngleY());
								glutPostRedisplay();
								break;
		}
	}
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
    int num_els = 8;
    Point maxs[num_els - 1];
    Point mins[num_els - 1];
    // Small model exterms
    models[0].getBoxPoints(min, max);
    // Floor exterms
    mins[0].x = -5;
    mins[0].y = 0.0;
    mins[0].z = -5;
    maxs[0].x = 5;
    maxs[0].y = 0.0;
    maxs[0].z = 5;
    // Big wall exterms
    mins[1].x = -5.0;
    mins[1].y = 0.0;
    mins[1].z = -5;
    maxs[1].x = -4.8;
    maxs[1].y = 1.5;
    maxs[1].z = 5;
    // Big model exterms
    models[1].getBoxPoints(mins[2], maxs[2]);
    // Small wall exterms
    mins[3].x = 0.5;
    mins[3].y = 0.0;
    mins[3].z = -1.6;
    maxs[3].x = 4.5;
    maxs[3].y = 1.5;
    maxs[3].z = -1.4;
    //Snowman 1 extrems
    mins[4].x = 2.1;
    mins[4].y = 0.0;
    mins[4].z = 2.1;
    maxs[4].x = 2.9;
    maxs[4].y = 1.2;
    maxs[4].z = 2.9;
    //Snowman 2 extrems
    mins[5].x = -2.9;
    mins[5].y = 0.0;
    mins[5].z = 2.1;
    maxs[5].x = -2.1;
    maxs[5].y = 1.2;
    maxs[5].z = 2.9;
    //Snowman 3 extrems
    mins[6].x = -2.9;
    mins[6].y = 0.0;
    mins[6].z = -2.9;
    maxs[6].x = -2.1;
    maxs[6].y = 1.2;
    maxs[6].z = -2.1;

    for (int i = 0; i < num_els - 1; ++i) {
        if (mins[i].x < min.x) min.x = mins[i].x;
        if (maxs[i].x > max.x) max.x = maxs[i].x;
        if (mins[i].y < min.y) min.y = mins[i].y;
        if (maxs[i].y > max.y) max.y = maxs[i].y;
        if (mins[i].z < min.z) min.z = mins[i].z;
        if (maxs[i].z > max.z) max.z = maxs[i].z;
    }
    cout << max.x << " " << max.y << " " << max.z << endl;
    cout << min.x << " " << min.y << " " << min.z << endl;
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
	initPersonalCamVars();
	draw_walls = true;
    enable_light = true;
}

/**
  * Initializate the openGL envrionament
  */
void initGL() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glEnable(GL_NORMALIZE);
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
	glutSpecialFunc(onKeyboardSpecialPulse);

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
