#include "shape.h"

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "../include/point_cloud.h"


int shape_is_valid(const Shape *shape) {
	assert(NULL != shape);
	if (shape->type < 0 || shape->type >= NumberShapeType)
		return 0;
	if (NULL == shape->contains_function)
		return 0;
	if (NULL == shape->point_cloud_converter)
		return 0;
	return 1;
}

static Shape * shape_allocate(ShapeType type, color4 color, int(*contains_function)(double *, point3 *), PointCloud * (*point_cloud_converter)(int, color4, mat4, mat4, double, double, double, double *), double *args) {
	assert(0 <= type && type < NumberShapeType);
	assert(NULL != contains_function);
	assert(NULL != point_cloud_converter);
	Shape * shape = NULL;
	if (NULL == (shape = (Shape *) malloc(sizeof(Shape)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	shape->type = type;
	color4_copy(shape->color, color);
	shape->contains_function = contains_function;
	shape->point_cloud_converter = point_cloud_converter;
	shape->args = args;
	shape->x_scale = shape->y_scale = shape->z_scale = 1;
	return shape; 
}

static color4 * n_color (int size, const color4 color) {
	assert(size > 0);
	color4 *colors = NULL;
	int i;
	if (NULL == (colors = (color4 *) malloc(size * sizeof(color4)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < size; i++) {
		color4_copy(colors[i], color);
	}
	return colors;
}

static int contains_sphere(double *args, point3 *point){
	assert(NULL != point);
	return SQUARE((*point)[X]) + SQUARE((*point)[Y]) + SQUARE((*point)[Z]) <= 1.;	
}
 
static void min_max(double *a, double *b, double *c) {
	assert(NULL != a);
	assert(NULL != b);
	assert(NULL != c);
	double tmp;
	if (*a < *b) {
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
	if (*a < *c) {
		tmp = *a;
		*a = *c;
		*c = tmp;
	}	
	if (*b < *c) {
		tmp = *b;
		*b = *c;
		*c = tmp;
	}	
} 
 
double randd(double min, double max) {
    int r = rand();
    double delta = max - min;
    double tmp = r / (RAND_MAX + 1.0); /* generate up to but excluding max */
    return tmp * delta + min;
}

static PointCloud * point_cloud_sphere(int density, color4 color, mat4 transformations, mat4 norm_transformations, double x_scale, double y_scale, double z_scale, double *args) {
	assert(density > 0);
	double a = x_scale, b = y_scale, c = z_scale;
	min_max(&a, &b, &c);
	double sqr_c = SQUARE(c);
	int size;
	if (a == c) {
		size = density*4*PI*sqr_c;
	} else {
		double sqrt_sasc = sqrt(SQUARE(a) - sqr_c);
		size = density*2*PI*(sqr_c + ((b*sqr_c)/(sqrt_sasc)) + b*sqrt_sasc);
	}
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	int i;
	point3 *v = vrtx;
	vec3 *n = norm;
	double half_pi = PI/2;
	for (i = 0; i < size; i++) {
		/*double alpha = g3x_Rand_Delta(PI,PI);*/
		double alpha = randd(0, 2*PI);
		/*double phi = (g3x_Rand_Delta(half_pi,half_pi) + g3x_Rand_Delta(half_pi,half_pi))/2.;*/
		double phi = randd(0, PI) + randd(0, PI)/2;
		double sin_phi = sin(phi);
		(*n)[X] = (*v)[X] = cos(alpha)*sin_phi;
		(*n)[Y] = (*v)[Y] = sin(alpha)*sin_phi;
		(*n)[Z] = (*v)[Z] = cos(phi);
		mat4_product_point3(transformations, *v, *v);
		mat4_product_vec3(norm_transformations, *n, *n);
		n++;
		v++;
	}
	return point_cloud_allocate(vrtx, norm, n_color(size, color), size);
}

Shape * shape_sphere(color4 color) {
	return shape_allocate(Sphere,color,contains_sphere,point_cloud_sphere,NULL); 
}

static int contains_cube(double *args, point3 *point) {
	assert(NULL != point);
	double x = (*point)[X], y = (*point)[Y], z = (*point)[Z];
	if (x < 0.) 
		x = -x;
	if (y < 0.) 
		y = -y;
	if (z < 0.) 
		z = -z;
	return MAX3(x,y,z) <= 1.;
}

static PointCloud * point_cloud_cube(int density, color4 color, mat4 transformations, mat4 norm_transformations, double x_scale, double y_scale, double z_scale, double *args) {
	assert(density > 0);
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	int xface = 4*density*y_scale*z_scale;
	int yface = 4*density*x_scale*z_scale;
	int zface = 4*density*x_scale*y_scale;
	int size = 2*(xface + yface + zface);
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	double x,y,z;
	int i;
	point3 *v = vrtx;
	vec3 *n = norm;
	for (i = 0; i < zface; i++) {
		for (z = -1; z <= 1; z+=2) {
			/*x = g3x_Rand_Delta(0,1);*/
			x = randd(-1, 1);
			/*y = g3x_Rand_Delta(0,1);*/
			y = randd(-1, 1);
			(*n)[X] = 0; (*v)[X] = x;
			(*n)[Y] = 0; (*v)[Y] = y;
			(*n)[Z] = (*v)[Z] = z;
			mat4_product_point3(transformations, *v, *v); 
			mat4_product_vec3(norm_transformations, *n, *n);
			n++;
			v++;
		}
	}
	for (i = 0; i < yface; i++) {
		for (y = -1; y <= 1; y+=2) {
			/*x = g3x_Rand_Delta(0,1);
			z = g3x_Rand_Delta(0,1);*/
			x = randd(-1, 1);
			z = randd(-1, 1);
			(*n)[X] = 0; (*v)[X] = x;
			(*n)[Y] = (*v)[Y] = y;
			(*n)[Z] = 0; (*v)[Z] = z;
			mat4_product_point3(transformations, *v, *v); 
			mat4_product_vec3(norm_transformations, *n, *n);
			n++;
			v++;
		}
	}
	for (i = 0; i < xface; i++) {
		for (x = -1; x <= 1; x+=2) {
			/*y = g3x_Rand_Delta(0,1);
			z = g3x_Rand_Delta(0,1);*/
			y = randd(-1, 1);
			z = randd(-1, 1);
			(*n)[X] = (*v)[X] = x;
			(*n)[Y] = 0; (*v)[Y] = y;
			(*n)[Z] = 0; (*v)[Z] = z;
			mat4_product_point3(transformations, *v, *v); 
			mat4_product_vec3(norm_transformations, *n, *n);
			n++;
			v++;
		}
	}
	return point_cloud_allocate(vrtx, norm, n_color(size, color), size);
}

Shape * shape_cube(color4 color) {
	return shape_allocate(Cube,color,contains_cube,point_cloud_cube,NULL); 
}

static int contains_cylinder(double *args, point3 *point){
	assert(NULL != point);
	double z = (*point)[Z];
	if (z < 0.)
		z = -z;
	return (z <= 1.) && (SQUARE((*point)[X]) + SQUARE((*point)[Y]) <= 1.);
}

static PointCloud * point_cloud_cylinder(int density, color4 color, mat4 transformations, mat4 norm_transformations, double x_scale, double y_scale, double z_scale, double *args) {
	assert(density > 0);
	int face = density*PI*x_scale*y_scale;
	int side = density*2*z_scale*PI*sqrt(2*(SQUARE(x_scale) + SQUARE(y_scale)));
	int size = side + 2*face;
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	double z;
	int i;
	point3 *v = vrtx;
	vec3 *n = norm;
	for (i = 0; i < side; i++) {
		/*z = g3x_Rand_Delta(0,1);*/
		z = randd(-1, 1);
		/*double alpha = g3x_Rand_Delta(PI,PI);*/
		double alpha = randd(0, 2*PI);
		(*n)[X] = (*v)[X] = cos(alpha);
		(*n)[Y] = (*v)[Y] = sin(alpha);
		(*n)[Z] = 0; (*v)[Z] = z;
		mat4_product_point3(transformations, *v, *v); 
		mat4_product_vec3(norm_transformations, *n, *n);
		n++;
		v++;
	}
	for (z = -1; z <= 1; z+=2) {
		i = 0;
		while (i < face) {
			/*double x = g3x_Rand_Delta(0,1);
			double y = g3x_Rand_Delta(0,1);*/
			double x = randd(-1, 1);
			double y = randd(-1, 1);
			if (x*x + y*y > 1)
				continue;
			(*n)[X] = 0; (*v)[X] = x;
			(*n)[Y] = 0; (*v)[Y] = y;
			(*n)[Z] = (*v)[Z] = z;
			mat4_product_point3(transformations, *v, *v); 
			mat4_product_vec3(norm_transformations, *n, *n);
			n++;
			v++;
			i++;
		}
	}
	return point_cloud_allocate(vrtx, norm, n_color(size, color), size);
}

Shape * shape_cylinder(color4 color) {
	return shape_allocate(Cylinder,color,contains_cylinder,point_cloud_cylinder,NULL); 
}

static int contains_cone(double *args, point3 *point){
	assert(NULL != point);
	double z = (*point)[Z];
	if (z < 0.)
		z = -z;
	double rz = 1 - (*point)[Z];
	return (z <= 1.) && (SQUARE((*point)[X]) + SQUARE((*point)[Y]) <= SQUARE(rz)/4.);
}

static PointCloud * point_cloud_cone(int density, color4 color, mat4 transformations, mat4 norm_transformations, double x_scale, double y_scale, double z_scale, double *args) {
	assert(density > 0);
	double r = x_scale*y_scale;
	double b = density*PI*r;
	int face = b;
	int side = b*sqrt(1 + (4.*SQUARE(z_scale))/r); 
	int size = side + face;
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	int i;
	point3 *v = vrtx;
	vec3 *n = norm;
	for (i = 0; i < side; i++) {
		/*double z = 2*(1 - sqrt(g3x_Rand_Delta(0.5,0.5))) - 1;*/
		double z = 2*(1 - sqrt(randd(0, 1))) - 1;
		/*double alpha = g3x_Rand_Delta(PI,PI);*/
		double alpha = randd(0, 2*PI);
		double rz = (1-z)/2;
		double cos_alpha = cos(alpha);
		double sin_alpha = sin(alpha);
		(*n)[X] = cos_alpha; (*v)[X] = rz*cos_alpha;
		(*n)[Y] = sin_alpha; (*v)[Y] = rz*sin_alpha;
		(*n)[Z] = 1; (*v)[Z] = z;
		mat4_product_point3(transformations, *v, *v); 
		mat4_product_vec3(norm_transformations, *n, *n);
		n++;
		v++;
	}
	i = 0;
	while (i < face) {
		/*double x = g3x_Rand_Delta(0,1);*/
		double x = randd(-1, 1);
		/*double y = g3x_Rand_Delta(0,1);*/
		double y = randd(-1, 1);
		if (x*x + y*y > 1)
			continue;
		(*n)[X] = 0; (*v)[X] = x;
		(*n)[Y] = 0; (*v)[Y] = y;
		(*n)[Z] = (*v)[Z] = -1;
		mat4_product_point3(transformations, *v, *v);
		mat4_product_vec3(norm_transformations, *n, *n);
		n++;
		v++;
		i++;
	}
	return point_cloud_allocate(vrtx, norm, n_color(size, color), size);
}

Shape * shape_cone(color4 color) {
	return shape_allocate(Cone,color,contains_cone,point_cloud_cone,NULL); 
}

static int contains_torus(double *args, point3 *point) {
	assert(NULL != point);
	assert(NULL != args);
	double r = args[0];
	double sqrx_sqry = SQUARE((*point)[X]) + SQUARE((*point)[Y]);
	double s = (sqrx_sqry + SQUARE((*point)[Z]) + 1 - SQUARE(r));
	return SQUARE(s) <= 4*sqrx_sqry;
}

static PointCloud * point_cloud_torus(int density, color4 color, mat4 transformations, mat4 norm_transformations, double x_scale, double y_scale, double z_scale, double *args) {
	assert(density > 0);
	assert(NULL != args);
	double r = args[0];
	int size = density*2*SQUARE(PI)*sqrt((SQUARE(x_scale) + SQUARE(y_scale))/2.)*sqrt(2*(SQUARE(r*z_scale) + SQUARE(r*((x_scale+y_scale)/2))));
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	int i;
	point3 *v = vrtx;
	vec3 *n = norm;
	double d = 0.2;
	double delta = PI + d; 
	for (i = 0; i < size; i++) {
		/*double phi = (g3x_Rand_Delta(0,delta) + g3x_Rand_Delta(0,delta))/2.;*/
		double phi = randd(-delta, delta) + randd(-delta, delta);
		/*double alpha = g3x_Rand_Delta(PI,PI);*/
		double alpha = randd(0, 2*PI);
		double cos_phi = cos(phi);
		double sin_phi = sin(phi);
		double r_cos_phi = 1 + r*cos_phi;
		double cos_alpha = cos(alpha); 
		double sin_alpha = sin(alpha); 
		(*n)[X] = cos_phi*cos_alpha; (*v)[X] = r_cos_phi*cos_alpha;
		(*n)[Y] = cos_phi*sin_alpha; (*v)[Y] = r_cos_phi*sin_alpha;
		(*n)[Z] = sin_phi; (*v)[Z] = r*sin_phi;
		mat4_product_point3(transformations, *v, *v); 
		mat4_product_vec3(norm_transformations, *n, *n);
		n++;
		v++;
	}
	return point_cloud_allocate(vrtx, norm, n_color(size, color), size);
}

Shape * shape_torus(color4 color, double radius) {
	assert(radius > 0);
	double *args = NULL;
	if (NULL == (args = (double *) malloc(sizeof(double)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	args[0] = radius;
	return shape_allocate(Torus,color,contains_torus,point_cloud_torus,args); 
}

int shape_contains_point(const Shape *shape, const point3 *point) {
	assert(NULL != shape);
	assert(shape_is_valid(shape));
	assert(NULL != point);
	return shape->contains_function(shape->args, (point3 *) point);
}
	
PointCloud * shape_to_point_cloud(const Shape *shape, int density, mat4 transformations, mat4 norm_transformations) {
	assert(NULL != shape);
	assert(shape_is_valid(shape));
	assert(density > 0);
	color4 *color = (color4 *) &(shape->color);
	return shape->point_cloud_converter(density, *color, transformations, norm_transformations, shape->x_scale, shape->y_scale, shape->z_scale, shape->args);
}

void shape_free(Shape **shape) {
	assert(NULL != shape);
	assert(NULL != (*shape));
	free((*shape));
	(*shape) = NULL;
}

void shape_rescale(Shape *shape, double x, double y, double z) {
	assert(NULL != shape);
	assert(shape_is_valid(shape));
	assert(x > 0);
	assert(y > 0);
	assert(z > 0);
	shape->x_scale *= x;
	shape->y_scale *= y;
	shape->z_scale *= z;
}
