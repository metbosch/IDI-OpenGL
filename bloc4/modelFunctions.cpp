#ifndef __MODEL_FUNCTIONS__
#define __MODEL_FUNCTIONS__

#include "model.h"
#include <vector>
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


class ModelLoader {

private:

	#define PAINT_VERTEX(_j) {\
		if ((_j) < triangles[i].n.size() && norm_vertex) {\
				double* norm = &normal[triangles[i].n[(_j)]];\
				glNormal3f(*(norm), *(norm + 1), *(norm + 2));\
		}\
		else {\
				glNormal3f(triangles[i].normalC[0], triangles[i].normalC[1], triangles[i].normalC[2]);\
		}\
		int num = triangles[i].v[(_j)];\
		glVertex3f(vertex[num], vertex[num + 1], vertex[num + 2]);\
	}

	#define SET_MATERIAL(_mat) {\
		const float* colors = Materials[(_mat)].diffuse;\
		glColor3f(*colors, *(colors + 1), *(colors + 2));\
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Materials[(_mat)].ambient);\
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors);\
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Materials[(_mat)].specular);\
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Materials[(_mat)].shininess);\
	}

	/** MODEL VARIABLES **/
	Model model;
	double REF_TRANS_X, REF_TRANS_Y, REF_TRANS_Z, SCALE_REF;
	double TRANS_X, TRANS_Y, TRANS_Z;
	double INIT_MODEL_HEIGHT;
	double ANGLE_X, ANGLE_Y, ANGLE_Z;
	Point MAX_POINT, MIN_POINT;

	/**
	 * Calculates the initial scale and translation for model
	 */
	void calcModelVars() {
			calcExtremsModelVertex(MIN_POINT, MAX_POINT);

			SCALE_REF = INIT_MODEL_HEIGHT/max(MAX_POINT.x - MIN_POINT.x, MAX_POINT.y - MIN_POINT.y,
											MAX_POINT.z - MIN_POINT.z);
			REF_TRANS_X = -(MIN_POINT.x + MAX_POINT.x)/2.0;
			REF_TRANS_Y = -(MIN_POINT.y + MAX_POINT.y)/2.0;
			REF_TRANS_Z = -(MIN_POINT.z + MAX_POINT.z)/2.0;
	}


	/**
	  * Get the two vertex of model minimum container box
	  */
	void calcExtremsModelVertex(Point &min, Point &max) {
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

public:
	/**
	  * Default Constructor
	  * Load the model contained in a file and calc the associated variables
	  * @param file String with the name of continer file of model
	  * @param height Desired final height for the model
	  * @param place Desifed final Point of model
	  */
	ModelLoader (string file, double height, Point place) {
		INIT_MODEL_HEIGHT = height;
		TRANS_X = place.x;
		TRANS_Y = place.y;
		TRANS_Z = place.z;

		ANGLE_X = 0.0;
		ANGLE_Y = 0.0;
		ANGLE_Z = 0.0;

		model = Model();
		model.load(file);
		calcModelVars();
	}

	/**
	  * Draw the model
	  * @param norm_vertex Indicates if vertex normals car be used or not
	  */
	void draw(bool norm_vertex) {
		glPushMatrix();
		glTranslated(TRANS_X, TRANS_Y, TRANS_Z);
		glScaled(SCALE_REF, SCALE_REF, SCALE_REF);
		glRotated(ANGLE_Z, 0, 0, 1);
		glRotated(ANGLE_X, 1, 0, 0);
		glRotated(ANGLE_Y, 0, 1, 0);
		glTranslated(REF_TRANS_X, REF_TRANS_Y, REF_TRANS_Z);

		vector<Face> triangles = model.faces();
		vector<Vertex> vertex = model.vertices();
		vector<Vertex> normal = model.normals();
		int last_mat;
		if (0 < triangles.size()) {
			last_mat = triangles[0].mat;
			if (last_mat >= 0 && last_mat < Materials.size()) {
				SET_MATERIAL(last_mat);
			}
		}
		for (int i = 0; i < triangles.size(); ++i) {
			glBegin(GL_TRIANGLES);
			int mat = triangles[i].mat;
			if (mat >= 0 && mat < Materials.size() && mat != last_mat) {
				SET_MATERIAL(mat);
				last_mat = mat;
			}
			PAINT_VERTEX(0);
			PAINT_VERTEX(1);
			PAINT_VERTEX(2);
			glEnd();
		}
		glPopMatrix();
	}

	/**
	  * Draw the model
      * @param norm_vertex Indicates if vertex normals car be used or not
	  * @param light Function to be called before draw faces and after set top Matrix
	  */
	void draw(bool norm_vertex, void (* light)()) {
		glPushMatrix();
		glTranslated(TRANS_X, TRANS_Y, TRANS_Z);
		light();
		glScaled(SCALE_REF, SCALE_REF, SCALE_REF);
		glRotated(ANGLE_Z, 0, 0, 1);
		glRotated(ANGLE_X, 1, 0, 0);
		glRotated(ANGLE_Y, 0, 1, 0);
		glTranslated(REF_TRANS_X, REF_TRANS_Y, REF_TRANS_Z);
		vector<Face> triangles = model.faces();
		vector<Vertex> vertex = model.vertices();
		vector<Vertex> normal = model.normals();
		int last_mat;
		if (0 < triangles.size()) {
			last_mat = triangles[0].mat;
			if (last_mat >= 0 && last_mat < Materials.size()) {
				SET_MATERIAL(last_mat);
			}
		}
		for (int i = 0; i < triangles.size(); ++i) {
			glBegin(GL_TRIANGLES);
			int mat = triangles[i].mat;
			if (mat >= 0 && mat < Materials.size() && mat != last_mat) {
				SET_MATERIAL(mat);
				last_mat = mat;
			}
			PAINT_VERTEX(0);
			PAINT_VERTEX(1);
			PAINT_VERTEX(2);
			glEnd();
		}
		glPopMatrix();
	}

	/**
	  * Get the points which determinate the model minimun container box
	  * @param ret_min Point to set the min values
	  * @param ret_max Point to set the max values
	  */
	void getBoxPoints(Point &ret_min, Point &ret_max) {
		ret_min.x = MIN_POINT.x*SCALE_REF;
		ret_max.x = MAX_POINT.x*SCALE_REF;
		ret_min.y = MIN_POINT.y*SCALE_REF;
		ret_max.y = MAX_POINT.y*SCALE_REF;
		ret_min.z = MIN_POINT.z*SCALE_REF;
		ret_max.z = MAX_POINT.z*SCALE_REF;
	}

	/**
	  * Set the model rotation angles arround its center
	  * @param an_x Angle for X axes
	  * @param an_y Angle for Y axes 
	  * @param an_z Angle for Z axes
	  */
	void setAngles(double an_x, double an_y, double an_z) {
		ANGLE_Z = an_z;
		ANGLE_Y = an_y;
		ANGLE_X = an_x;
	}

	/**
	  * Set the model rotation angles arround its center
	  * @param place Position to place the model
	  */
	void setPosition(Point place) {
		TRANS_X = place.x;
		TRANS_Y = place.y;
		TRANS_Z = place.z;
	}

	/**
	  * Increment the model rotation angles arround its center
	  * @param an_x Angle for X axes
	  * @param an_y Angle for Y axes
	  * @param an_z Angle for Z axes
	  */
	void increaseAngles(double an_x, double an_y, double an_z) {
		ANGLE_Z += an_z;
		ANGLE_Y += an_y;
		ANGLE_X += an_x;
	}

	/**
	  * Get the center point of the model
	  * @return Point where the center of the model was
	  */
	Point getPosition() {
		Point p = {TRANS_X, TRANS_Y, TRANS_Z};
		return p;
	}

	/**
	  * Get the Y axes rotation angle of the model
	  * @return Angle in radiants of Y axes
	  */
	double getAngleY() {
		return ANGLE_Y*TO_RADIANTS;
	}

	/**
	  * Make the model walk
	  * @param units Number of units to advance
	  */
	void walk(double units) {
		TRANS_X += sin(ANGLE_Y*TO_RADIANTS)*units;
		TRANS_Z += cos(ANGLE_Y*TO_RADIANTS)*units;
	}
};

#endif
