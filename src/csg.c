#include "tree.h"
#include "parser.h"
#include "point_cloud.h"
#include <GL/glut.h>
#include <time.h>

#define WINDOW_WIDTH (768)
#define WINDOW_HEIGHT (512)
#define BGCOLOR_R (0.22)
#define BGCOLOR_G (0.22)
#define BGCOLOR_B (0.22)
#define DISTANCE_NEAR (0.1)
#define DISTANCE_FAR (1000.0)
#define CAMERA_X (0)
#define CAMERA_Y (-3)
#define CAMERA_Z (2)
#define CAMERA_TARGET_X (0)
#define CAMERA_TARGET_Y (0)
#define CAMERA_TARGET_Z (0)
#define LIGHT_DIRECTION_X (0)
#define LIGHT_DIRECTION_Y (0)
#define LIGHT_DIRECTION_Z (1)
#define WINDOW_NAME ("CSG Tree Point Rendering")
#define LOW_TOKEN ("low")
#define LOW_DENSITY (3000)
#define MEDIUM_TOKEN ("medium")
#define MEDIUM_DENSITY (20000)
#define HIGH_TOKEN ("high")
#define HIGH_DENSITY (100000)

PointCloud *points_scene = NULL;

void display() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(60.0, w/h, DISTANCE_NEAR, DISTANCE_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
    	CAMERA_X, CAMERA_Y, CAMERA_Z,
		CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z,
		0, 0, 1
	);
	glClearColor(BGCOLOR_R, BGCOLOR_G, BGCOLOR_B, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    point_cloud_draw(points_scene);
    glutSwapBuffers();
}

void init() {
	GLfloat black[] = {0.2, 0.2, 0.2, 1};
	GLfloat white[] = {0.75, 0.75, 0.75, 1};
	GLfloat direction[] = {LIGHT_DIRECTION_X, LIGHT_DIRECTION_Y, LIGHT_DIRECTION_Z, 0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
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

	srand(time(NULL));

	FILE *f = NULL;
	if (NULL == (f = fopen(filescene, "r"))) {
		fprintf(stderr, "can not open file '%s'\n", filescene);
		exit(EXIT_FAILURE);
	}
	Tree scene = parse_tree(f);
	fclose(f);

	points_scene = tree_to_point_cloud(scene, density);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    point_cloud_free(&points_scene);
    tree_free(&scene);

}
