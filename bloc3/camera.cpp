#ifndef __CAMERA_FUNCTIONS__
#define __CAMERA_FUNCTIONS__

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

/** CONSTATS VALUES **/
const int CAM_MODES[] = {GL_MODELVIEW, GL_PROJECTION};

/** CAMERA VARIABLES **/
double CAM_ANGLE_X, CAM_ANGLE_Y, CAM_ANGLE_Z;
double DIST;
Point VRP;
unsigned char CAM_MODE;

void setCameraMatrix() {
    glLoadIdentity();
    glTranslated(0.0, 0.0, -DIST);
    glRotated(-CAM_ANGLE_Z, 0, 0, 1);
    glRotated( CAM_ANGLE_X, 1, 0, 0);
    glRotated(-CAM_ANGLE_Y, 0, 1, 0);
    glTranslated(-VRP.x, -VRP.y, -VRP.z);
}

void setMatrixMode() {
	glMatrixMode(CAM_MODES[CAM_MODE]);
}

void changeCameraMode() {
	++CAM_MODE;
    CAM_MODE %= (sizeof(CAM_MODES)/sizeof(*CAM_MODES));
    glMatrixMode(CAM_MODES[CAM_MODE]);
}

string getStrCameraMode() {
	return (CAM_MODE ? "GL_PROJECTION" : "GL_MODELVIEW");
}

int getCameraMode() {
	return CAM_MODE;
}

void initCamera() {
	CAM_MODE = 0;
    CAM_ANGLE_Y = CAM_ANGLE_Z = CAM_ANGLE_X = 0.0;
    VRP.x = VRP.y = VRP.z = 0.0;
}

#endif