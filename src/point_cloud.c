#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <assert.h>
#include "point_cloud.h"

int point_cloud_is_valid(const PointCloud *point_cloud) {
	assert(NULL != point_cloud);
	if (point_cloud->size < 0)
		return 0;
	if (NULL == point_cloud->vrtx)
		return 0;
	if (NULL == point_cloud->norm)
		return 0;
	if (NULL == point_cloud->colors)
		return 0;
	return 1;
} 

PointCloud * point_cloud_allocate(point3 *vrtx, vec3 *norm, color4 *colors, int size) {
	assert(NULL != vrtx);
	assert(NULL != norm);
	assert(NULL != colors);
	assert(size >= 0);
	PointCloud *point_cloud = NULL;
	if (NULL == (point_cloud = (PointCloud *) malloc(sizeof(PointCloud)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	point_cloud->vrtx = vrtx; 
	point_cloud->norm = norm;
	point_cloud->colors = colors;
	point_cloud->size = size;
	return point_cloud;
}

void set_material(color4 color) {
	GLfloat specular[] = {0.6, 0.6, 0.6, 1};
	GLfloat shininess = 30;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void point_cloud_draw(const PointCloud *point_cloud) {
	assert(NULL != point_cloud);
	assert(point_cloud_is_valid(point_cloud));
	point3 *v = point_cloud->vrtx;
	vec3 *n = point_cloud->norm;
	color4 *c = point_cloud->colors;
	point3 *max = point_cloud->vrtx + point_cloud->size;
	glPointSize(POINT_SIZE);
	glBegin(GL_POINTS);
	glEnable(GL_COLOR_MATERIAL);
	while(v < max) {
		set_material((*c));
		glNormal3dv(*n);
		glVertex3dv(*v);
		n++;
		v++;
		c++;
	}
	glDisable(GL_COLOR_MATERIAL);
    glEnd();
} 

void point_cloud_free (PointCloud **point_cloud) {
	assert(NULL != point_cloud);
	assert(NULL != (*point_cloud));
	assert(point_cloud_is_valid(*point_cloud));
	free((*point_cloud)->vrtx);
	free((*point_cloud)->norm);
	free((*point_cloud)->colors);
	free((*point_cloud));
	(*point_cloud) = NULL;
}
