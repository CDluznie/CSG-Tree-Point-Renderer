#include "shape.h"

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "point_cloud.h"


double random_double(double min, double max) {
    return (max - min) * ((double) rand() / (double) RAND_MAX) + min;
}

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

static int contains_sphere(double *args, point3 *point) {
	assert(NULL != point);
	return SQUARE(point3_get_x((*point))) + SQUARE(point3_get_y((*point))) + SQUARE(point3_get_z((*point))) <= 1.;
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
		double alpha = random_double(0, 2*PI);
		double phi = random_double(0, PI) + random_double(0, PI)/2;
		double sin_phi = sin(phi);
		vec3_set((*n), cos(alpha)*sin_phi, sin(alpha)*sin_phi, cos(phi));
		point3_set((*v), cos(alpha)*sin_phi, sin(alpha)*sin_phi, cos(phi));
		mat4_product_point3((*v), transformations, (*v));
		mat4_product_vec3((*n), norm_transformations, (*n));
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
	double x = point3_get_x((*point));
	double y = point3_get_y((*point));
	double z = point3_get_z((*point));
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
			x = random_double(-1, 1);
			y = random_double(-1, 1);
			vec3_set((*n), 0, 0, z);
			point3_set((*v), x, y, z);
			mat4_product_point3((*v), transformations, (*v));
			mat4_product_vec3((*n), norm_transformations, (*n));
			n++;
			v++;
		}
	}
	for (i = 0; i < yface; i++) {
		for (y = -1; y <= 1; y+=2) {
			x = random_double(-1, 1);
			z = random_double(-1, 1);
			vec3_set((*n), 0, y, 0);
			point3_set((*v), x, y, z);
			mat4_product_point3((*v), transformations, (*v));
			mat4_product_vec3((*n), norm_transformations, (*n));
			n++;
			v++;
		}
	}
	for (i = 0; i < xface; i++) {
		for (x = -1; x <= 1; x+=2) {
			y = random_double(-1, 1);
			z = random_double(-1, 1);
			vec3_set((*n), x, 0, 0);
			point3_set((*v), x, y, z);
			mat4_product_point3((*v), transformations, (*v));
			mat4_product_vec3((*n), norm_transformations, (*n));
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
	double z = point3_get_z((*point));
	if (z < 0.)
		z = -z;
	return (z <= 1.) && (SQUARE(point3_get_x((*point))) + SQUARE(point3_get_y((*point))) <= 1.);
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
		z = random_double(-1, 1);
		double alpha = random_double(0, 2*PI);
		vec3_set((*n), cos(alpha), sin(alpha), 0);
		point3_set((*v), cos(alpha), sin(alpha), z);
		mat4_product_point3((*v), transformations, (*v));
		mat4_product_vec3((*n), norm_transformations, (*n));
		n++;
		v++;
	}
	for (z = -1; z <= 1; z+=2) {
		i = 0;
		while (i < face) {
			double x = random_double(-1, 1);
			double y = random_double(-1, 1);
			if (x*x + y*y > 1)
				continue;
			vec3_set((*n), 0, 0, z);
			point3_set((*v), x, y, z);
			mat4_product_point3((*v), transformations, (*v));
			mat4_product_vec3((*n), norm_transformations, (*n));
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
	double z = point3_get_z((*point));
	if (z < 0.)
		z = -z;
	double rz = 1 - point3_get_z((*point));
	return (z <= 1.) && (SQUARE(point3_get_x((*point))) + SQUARE(point3_get_y((*point))) <= SQUARE(rz)/4.);
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
		double z = 2*(1 - sqrt(random_double(0, 1))) - 1;
		double alpha = random_double(0, 2*PI);
		double rz = (1-z)/2;
		double cos_alpha = cos(alpha);
		double sin_alpha = sin(alpha);
		vec3_set((*n), cos_alpha, sin_alpha, 1);
		point3_set((*v), rz*cos_alpha, rz*sin_alpha, z);
		mat4_product_point3((*v), transformations, (*v));
		mat4_product_vec3((*n), norm_transformations, (*n));
		n++;
		v++;
	}
	i = 0;
	while (i < face) {
		double x = random_double(-1, 1);
		double y = random_double(-1, 1);
		if (x*x + y*y > 1)
			continue;
		vec3_set((*n), 0, 0, -1);
		point3_set((*v), x, y, -1);
		mat4_product_point3((*v), transformations, (*v));
		mat4_product_vec3((*n), norm_transformations, (*n));
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
	double sqrx_sqry = SQUARE(point3_get_x((*point))) + SQUARE(point3_get_y((*point)));
	double s = (sqrx_sqry + SQUARE(point3_get_z((*point))) + 1 - SQUARE(r));
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
		double phi = random_double(-delta, delta) + random_double(-delta, delta);
		double alpha = random_double(0, 2*PI);
		double cos_phi = cos(phi);
		double sin_phi = sin(phi);
		double r_cos_phi = 1 + r*cos_phi;
		double cos_alpha = cos(alpha); 
		double sin_alpha = sin(alpha);
		vec3_set((*n), cos_phi*cos_alpha, cos_phi*sin_alpha, sin_phi);
		point3_set((*v), r_cos_phi*cos_alpha, r_cos_phi*sin_alpha, r*sin_phi);
		mat4_product_point3((*v), transformations, (*v));
		mat4_product_vec3((*n), norm_transformations, (*n));
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
