#include <iostream>
using namespace std;
#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include<GL/gl.h>
    #include<GL/freeglut.h>
#endif

struct point {
	float x, y, z;
};

int MODE = 0;
point TRIANGLE[3];
int CONT = 0;
	
void help() {
	cout << endl;
	cout << "---------------> >> HELP INFORMATION << <--------------" << endl;
	cout << " Set the response of mouse events on window" << endl;
	cout << " -> Press 'f' to set background color editor (default)" << endl;
	cout << " -> Press 't' to set triangle editor" << endl;
	cout << "    Click on window to set triangle corners" << endl;
	cout << "    Start with upper corner and turn anti clock" << endl;
	cout << " -> Press ESC to exit" << endl;
	cout << "-------------------------------------------------------" << endl;
}

void close() {
	exit(1);
}

int min (int a, int b) {
    if (a > b) return b;
    return a;
}

int max (int a, int b) {
    if (a < b) return b;
    return a;
}

void refresh () {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3f(TRIANGLE[0].x, TRIANGLE[0].y, TRIANGLE[0].z);
	glColor3f(0, 1, 0);
        glVertex3f(TRIANGLE[1].x, TRIANGLE[1].y, TRIANGLE[1].z);
	glColor3f(0, 0, 1);
        glVertex3f(TRIANGLE[2].x, TRIANGLE[2].y, TRIANGLE[2].z);
    glEnd();
    glutSwapBuffers();
}

void resize(int height, int width) {
    int small = min(height, width);
    glViewport(max(0, (height - width)/2), max(0, (width - height)/2), small, small);
//    cerr << max(0, (height - width)/2) << "\t" << max(0, (width - height)/2) << "\t" << small << "\t" << small << endl;
}

void setViewportClick(int height, int width, int &x, int &y) {
    int height_ini = max(0, (height - width)/2);
    int width_ini = max(0, (width - height)/2);
    x -= width_ini;
    y -= height_ini;
}

void mouseClick(int buton, int evnt, int x, int y) {
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	float width = glutGet(GLUT_WINDOW_WIDTH);
	if (MODE == 1) {
		if (CONT > 2) {
			CONT = 0;
			MODE = 0;
		}
		else if (evnt == 0){
			y = (height - y);
			cout << x << "\t" << y << "\t";
			setViewportClick(height, width, x, y);
			cout << x << "\t" << y << endl;
			int small = min(height, width);
			TRIANGLE[CONT].x = x*2.0/small - 1.0;
			TRIANGLE[CONT].y = y*2.0/small - 1.0;
			++CONT;
		}
		glutPostRedisplay();
	}
}

void mouseMotion(int x, int y) {
	float height = glutGet(GLUT_WINDOW_WIDTH);
	float width = glutGet(GLUT_WINDOW_WIDTH);
	if (MODE == 0) {
		glClearColor(x*1.0/width, y*1.0/height, 0, 1);
		glutPostRedisplay();
	}
}

void keyboardPulse(unsigned char key, int x, int y) {
	if (key == 'h') help();
	else if ((int)key == 27) close();
	else if (key == 'f') MODE = 0;
	else if (key == 't') MODE = 1;
}

void initGL () {
    glClearColor(1, 1, 0, 1);
}

void initTriangle() {
	TRIANGLE[0].x = -0.5;
	TRIANGLE[0].y = -1.0/3.0;
	TRIANGLE[0].z =  0.0;
	TRIANGLE[1].x =  0.5;
	TRIANGLE[1].y = -1.0/3.0;
	TRIANGLE[1].z =  0.0;
	TRIANGLE[2].x =  0.0;
	TRIANGLE[2].y =  2.0/3.0;
	TRIANGLE[2].z =  0.0;
}

int main (int argc, const char *argv[]) {
    // Inicialitzacions GLUT
    glutInit(&argc, ((char **)argv));
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practica 0");

    // Registre de Callbacks
    glutDisplayFunc(refresh);
    glutReshapeFunc(resize);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboardPulse);

    // Inicialitzacio openGL
	initTriangle();
    initGL();

    // Bucle de events GLUT
    glutMainLoop();
    return 0;
}
