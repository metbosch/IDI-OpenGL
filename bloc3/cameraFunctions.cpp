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
const string CAM_MODES[] = {"PERSPECTIVE", "AXONOMETRIC"};

/** CAMERA VARIABLES **/
double CAM_ANGLE_X, CAM_ANGLE_Y, CAM_ANGLE_Z;
double DIST;
Point VRP;
unsigned char CAM_MODE;

/**
  * Set Glut Matrix Mode according to current value
  */
void setMatrixMode() {


	glMatrixMode(CAM_MODES[CAM_MODE]);
}

/**
  * Set in the stack top a new Matrix according to current values
  */
void setCameraMatrix() {
    glLoadIdentity();
    glTranslated(0.0, 0.0, -DIST);
    glRotated(-CAM_ANGLE_Z, 0, 0, 1);
    glRotated( CAM_ANGLE_X, 1, 0, 0);
    glRotated(-CAM_ANGLE_Y, 0, 1, 0);
    glTranslated(-VRP.x, -VRP.y, -VRP.z);
}




/**
  * Change Glut Matrix Mode. Possible values are: GL_MODELVIEW and GL_PROJECTION
  */
void changeCameraMode() {
    ++CAM_MODE;
    CAM_MODE %= (sizeof(CAM_MODES)/sizeof(*CAM_MODES));
}

/**
  * Get string identifier of current Glut Matrix Mode value
  */
string getStrCameraMode() {
	return (CAM_MODE ? "GL_PROJECTION" : "GL_MODELVIEW");
}

/**
  * Get integer identifier of current Glut Matrix Mode value
  */
int getCameraMode() {
	return CAM_MODE;
}

/**
  * Initializate the camera variables to default values
  */
void initCamera() {
	CAM_MODE = 0;
    CAM_ANGLE_Y = 0.0;
    CAM_ANGLE_Z = 0.0;
    CAM_ANGLE_X = 2.0;
    VRP.x = 0.0;
    VRP.y = 0.0;
    VRP.z = 0.0;
}

/**
  * Initializate the camera variables to specified values
  */
void initCamera(double x_cam, double y_cam, double z_cam,
                double x_vrp, double y_vrp, double z_vrp) {
    CAM_MODE = 0;
    CAM_ANGLE_Y = y_cam;
    CAM_ANGLE_Z = z_cam;
    CAM_ANGLE_X = x_cam;
    VRP.x = x_vrp;
    VRP.y = y_vrp;
    VRP.z = z_vrp;
}

/**
  * Set the angles of the camera, called Euler Angles
  */
void setEulerAngles(double z_cam, double y_cam) {
    CAM_ANGLE_Y = y_cam;
    CAM_ANGLE_Z = z_cam;
}

/**
  * Increment the angles of the camera acordinf to parameters
  */
void incrementEulerAngles(double x_cam, double y_cam, double z_cam) {
    CAM_ANGLE_Y += y_cam;
    CAM_ANGLE_Z += z_cam;
    CAM_ANGLE_X += x_cam;
}

/**
  * Set the VRP the with the specified values
  */
void setVRP(double x_move, double y_move, double z_move) {
    VRP.x = x_move;
    VRP.y = y_move;
    VRP.z = z_move;
}

/**
 * Move the VRP the with the specified values
 * @param x_move Move value for x axis
 * @param y_move Move value for y axis
 * @param z_move Move value for z axis
 */
void moveVRP(double x_move, double y_move, double z_move) {
    VRP.x += x_move;
    VRP.y += y_move;
    VRP.z += z_move;
}

/**
  * Set the dist between camera and VRP
  * @param d Distance to be setted
  */
void setDist(double d) {
    DIST = d;
}

#endif
