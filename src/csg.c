#include "tree.h"
#include "parser.h"

#include <GL/glut.h>
#include "../include/point_cloud.h"



#define WINDOW_WIDTH (768)
#define WINDOW_HEIGHT (512)
#define BGCOLOR_R (0.22)
#define BGCOLOR_G (0.22)
#define BGCOLOR_B (0.22)
#define DISTANCE_NEAR (0.1)
#define DISTANCE_FAR (1000.0)
#define WINDOW_NAME ("CSG Tree Point Rendering")
#define LOW_TOKEN ("low")
#define LOW_DENSITY (5000)
#define MEDIUM_TOKEN ("medium")
#define MEDIUM_DENSITY (20000)
#define HIGH_TOKEN ("high")
#define HIGH_DENSITY (100000)


PointCloud *points_scene = NULL;
int angle = 0;

void timer(int value) {
	unsigned int delay = 20;
    angle = (angle + 1)%360;
    glutTimerFunc(delay, timer, value);
    glutPostRedisplay();
}

void display() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(60.0, w/h, DISTANCE_NEAR, DISTANCE_FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-1, 3, 2, 0, 0, 0, 0, 0, 1);


	glClearColor(BGCOLOR_R, BGCOLOR_G, BGCOLOR_B, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glRotated(angle, 0, 0, 1);

	
	point_cloud_draw(points_scene);
    glutSwapBuffers();
    
}

int main (int argc, char *argv[]) {
	
	if (argc != 3) {
		fprintf(stderr, "error bad arguments\nusage : %s scene_file density\n", argv[0]);
		exit(EXIT_FAILURE);
	}
  
	char *filescene = argv[1];
	
	int density;
	if (strcmp(argv[2],LOW_TOKEN) == 0) {
		density = LOW_DENSITY;
	} else if (strcmp(argv[2],MEDIUM_TOKEN) == 0) {	
		density = MEDIUM_DENSITY;
	} else if (strcmp(argv[2],HIGH_TOKEN) == 0) {	
		density = HIGH_DENSITY;
	} else {
		fprintf(stderr, "error bad value for argument density\ndensity : %s | %s | %s\n", LOW_TOKEN, MEDIUM_TOKEN, HIGH_TOKEN);
		exit(EXIT_FAILURE);
	}
  
	#ifndef NDEBUG
	printf("Debug mode\n");
	#endif
	
	FILE *f = NULL;
	if (NULL == (f = fopen(filescene, "r"))) {
		fprintf(stderr, "can not open file '%s'\n", filescene);
		exit(EXIT_FAILURE);
	}
	Tree scene = parse_tree(f); 
	fclose(f);
	
	points_scene = tree_to_point_cloud(scene, density);
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();

}
