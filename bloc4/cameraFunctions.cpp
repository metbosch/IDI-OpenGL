#ifndef __CAMERA_FUNCTIONS__
#define __CAMERA_FUNCTIONS__

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
    #include <GL/glu.h>
#endif
#include <cmath>

/** TYPE DEFINITIONS **/
#ifndef __POINT_DEF__
#define __POINT_DEF__

struct Point {
    double x, y, z;
};

#endif

/** CONSTATS VALUES **/
#define PI (3.14159)

/** CAMERA VARIABLES **/
double CAM_ANGLE_X, CAM_ANGLE_Y, CAM_ANGLE_Z;
double DIST, RADIUS, HEIGHT, WIDTH, ZOOM, alfa_ini, aperture;
double zNear = 0.001;
double zFar = 10;
Point VRP;
unsigned char CAM_MODE;

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

void updateCamera(double new_height, double new_width) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (CAM_MODE) {
        if (new_width/new_height <= 1) {
            double tmp = (RADIUS*new_height/new_width); 
            glOrtho(-RADIUS*ZOOM, RADIUS*ZOOM, -tmp*ZOOM, tmp*ZOOM, zNear, zFar);
        }
        else {
            double tmp = (RADIUS*new_width/new_height); 
            glOrtho(-tmp*ZOOM, tmp*ZOOM, -RADIUS*ZOOM, RADIUS*ZOOM, zNear, zFar);
        }
    }
    else {
        if (new_width/new_height < 1) {
            aperture = atan(tan(alfa_ini)*new_height/new_width)*360.0/PI;
            gluPerspective(aperture*ZOOM, new_width/new_height, zNear, zFar);
        }
        else {
            gluPerspective(aperture*ZOOM, new_width/new_height, zNear, zFar);
        }
    }
    glMatrixMode(GL_MODELVIEW);
    HEIGHT = new_height;
    WIDTH = new_width;
}


/**
  * Change the camera type between Orthogonal and Prespective
  */
void changeCameraType() {
    CAM_MODE = ++CAM_MODE%2;
    updateCamera(HEIGHT, WIDTH);
}

/**
  * Get string identifier of current Glut Matrix Mode value
  */
string getStrCameraMode() {
	return (CAM_MODE ? "AXONOMETRIC" : "PRESPECTIVE");
}

/**
  * Initializate the camera variables to default values
  */
void initCamera(double rad) {
    glMatrixMode(GL_MODELVIEW);
	CAM_MODE = 0;
    DIST = 0.1;
    CAM_ANGLE_Y = 0.0;
    CAM_ANGLE_Z = 0.0;
    CAM_ANGLE_X = 2.0;
    VRP.x = 0.0;
    VRP.y = 0.0;
    VRP.z = 0.0;
    RADIUS = rad;
    ZOOM = 1.0;
    alfa_ini = asin(RADIUS/DIST);
	aperture = alfa_ini*360.0/PI;
}

/**
  * Initializate the camera variables to specified values
  */
void initCamera(double x_cam, double y_cam, double z_cam, Point vrp, double rad) {
    glMatrixMode(GL_MODELVIEW);
    CAM_MODE = 0;
    DIST = 0.1;
    CAM_ANGLE_Y = y_cam;
    CAM_ANGLE_Z = z_cam;
    CAM_ANGLE_X = x_cam;
    VRP = vrp;
    RADIUS = rad;
    ZOOM = 1.0;
    alfa_ini = asin(RADIUS/DIST);
	aperture = alfa_ini*360.0/PI;
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
  * @param near Distance to start painting objects
  * @param far Distance to stop painting objects
  */
void setCamDist(double d, double near, double far) {
    DIST = d;
    zNear = near;
    zFar = far;
    alfa_ini = asin(RADIUS/DIST);
	aperture = alfa_ini*360.0/PI;
}

/**
  * Set the dist between camera and VRP
  * @param d Value to be applied
  */
void incrementCamDist(double d) {
    DIST += d;
}

double getCamDist() {
  return DIST;
}

double getCamAperture() {
  return aperture;
}

void zoomIn() {
  ZOOM *= 0.9;
}

void zoomOut() {
  ZOOM *= 1.1;
}

/* --------------------- PERSONAL CAM ---------------------- */

/** VARIABLES **/
double alfa, beta, velocity;

void initPersonalCamVars() {
  alfa = 0.0;
  beta = 0.0;
  velocity = 0.1;
}

void initPersonalCam(double height, double width) {
  glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, width/height, 0.1, 2*RADIUS);
  glMatrixMode(GL_MODELVIEW);
  
}

void updatePersonalCam(Point pos, double angle) {
    Point vrp;
    vrp.x = pos.x + sin(angle + alfa*TO_RADIANTS);
    vrp.y = 2.0*pos.y + sin(beta*TO_RADIANTS);
    vrp.z = pos.z + cos(angle + alfa*TO_RADIANTS);
    glLoadIdentity();
    gluLookAt(pos.x, 2.0*pos.y, pos.z,
              vrp.x, vrp.y, vrp.z,
              0, 1, 0);
}

void resetAlfaBeta() {
  alfa = 0.0;
  beta = 0.0;
}

void increaseVelocity() {
    velocity *= 1.1;
}

void decreaseVelocity() {
    velocity *= 0.9;
}

void increaseAlfa() {
    alfa += 3;
}

void decreaseAlfa() {
    alfa -= 3;
}

void increaseBeta() {
    beta += beta > 90 ? 0 : 3;
}

void decreaseBeta() {
    beta -= beta < -90 ? 0 : 3;
}

double getAlfa() {
  return alfa;
}

double getBeta() {
  return beta;
}

/* ----------------- END OF PERSONAL CAM ------------------- */

#endif
