/**
 * \file 
 * \brief 
 */
 
#ifndef __TYPES__H__
#define __TYPES__H__

#include <math.h>
#include <string.h>

typedef float color4[4];

typedef double coord3[3];
typedef coord3 point3, vec3;

typedef double mat4[16];

#define PI 3.1415926535897932384626433832795

#define SQUARE(a) ((a)*(a))
#define MIN3(a,b,c) (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c)))
#define MAX3(a,b,c) (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c)))

#define color4_set(color, r, g, b, a) ((color)[0]=(r),\
                            	   (color)[1]=(g),\
								   (color)[2]=(b),\
								   (color)[3]=(a))

#define color4_copy(color1, color2) ((color1)[0]=(color2)[0],\
                            		 (color1)[1]=(color2)[1],\
									 (color1)[2]=(color2)[2],\
									 (color1)[3]=(color2)[3])

#define color4_scale(color, s) {(color)[0]*s,\
                               (color)[1]*s,\
                               (color)[2]*s,\
                               (color)[3]*s}

#define coord3_get_x(c) ((c)[0])

#define coord3_get_y(c) ((c)[1])

#define coord3_get_z(c) ((c)[2])

#define coord3_set(c, x, y, z) ((c)[0]=(x),\
                            	(c)[1]=(y),\
								(c)[2]=(z))

#define coord3_copy(c1, c2) ((c1)[0]=(c2)[0],\
                             (c1)[1]=(c2)[1],\
							 (c1)[2]=(c2)[2])

#define coord3_scale(c, s) {(c)[0]*s,\
                         	(c)[1]*s,\
							(c)[2]*s}

#define point3_get_x(p) (coord3_get_x(p))

#define point3_get_y(p) (coord3_get_y(p))

#define point3_get_z(p) (coord3_get_z(p))

#define point3_copy(p1, p2) (coord3_copy(p1, p2))

#define point3_set(p, x, y, z) (coord3_set(p, x, y, z))

#define point3_from_to_vector(p1, p2) {(p2)[0] - (p1)[0],\
									   (p2)[1] - (p1)[1],\
									   (p2)[2] - (p1)[2]}

#define vec3_get_x(v) (coord3_get_x(v))

#define vec3_get_y(v) (coord3_get_y(v))

#define vec3_get_z(v) (coord3_get_z(v))

#define vec3_copy(v1, v2) (coord3_copy(v1, v2))

#define vec3_set(v, x, y, z) (coord3_set(v, x, y, z))

#define vec3_scale(v, s) (coord3_scale(v, s))

#define vec3_dot(u, v) ((u)[0]*(v)[0]+(u)[1]*(v)[1]+(u)[2]*(v)[2])

#define vec3_cross(result, u, v) ((result)[0]=(u)[1]*(v)[2]-(u)[2]*(v)[1],\
                                  (result)[1]=(u)[2]*(v)[0]-(u)[0]*(v)[2],\
								  (result)[2]=(u)[0]*(v)[1]-(u)[1]*(v)[0])

#define vec3_norm(v) (sqrt(vec3_dot(v,v)))

void vec3_normalize(vec3 v);

#define mat4_set(m, col, line, val) ((m)[4*(col)+(line)]=(val))

#define mat4_get(m, col, line) ((m)[4*(col)+(line)])

#define mat4_identity {1.,0.,0.,0.,\
                       0.,1.,0.,0.,\
                       0.,0.,1.,0.,\
                       0.,0.,0.,1.}

#define mat4_set_identity(A) (memset((A), 0, sizeof(mat4)),\
                              (A)[0]=(A)[5]=(A)[10]=(A)[15]=1)

void mat4_translation(mat4 A, double tx, double ty, double tz);
  
void mat4_homothety(mat4 A, double hx, double hy, double hz);
  
void mat4_rotation_x(mat4 A, double alpha);
  
void mat4_rotation_y(mat4 A, double alpha);

void mat4_rotation_z(mat4 A, double alpha);

void mat4_product_mat4(mat4 C, mat4 A, mat4 B);

void mat4_product_vec3(vec3 W, mat4 A, vec3 V);

void mat4_product_point3(point3 Q, mat4 A, point3 P);

#endif
