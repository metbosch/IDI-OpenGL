#ifndef __MODEL_FUNCTIONS__
#define __MODEL_FUNCTIONS__

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
#ifndef __POINT_DEF__
#define __POINT_DEF__

struct Point {
    double x, y, z;
};

#endif

/** GLOBAL MODEL VARIABLES **/
Model model;
double REF_TRANS_X, REF_TRANS_Y, REF_TRANS_Z, SCALE_REF;
double TRANS_X, TRANS_Y, TRANS_Z;
double INIT_MODEL_HEIGHT;

/* ------------------ AUXILIAR FUNCTIONS ------------------*/

int min (int a, int b) {
    return a - ((a - b) & -(b < a));
}

double max (double a, double b) {
    if (a > b) return a;
    return b;
}

double max (double a, double b, double c) {
    if (max(a, b) > max(b, c)) return a;
    else if (max(a, b) < max(b, c)) return c;
    else return b;
}

/* -------------- END OF AUXILIAR FUNCTIONS ---------------*/

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

#endif