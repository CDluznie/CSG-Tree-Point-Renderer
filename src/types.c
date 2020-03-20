#include "types.h"

#include <math.h>
#include <stdio.h>

void vec3_normalize(vec3 v) {
	double norm = vec3_norm(v);
	vec3_set(
		v,
		vec3_get_x(v)/norm,
		vec3_get_y(v)/norm,
		vec3_get_z(v)/norm
	);
}

void mat4_translation(mat4 A, double tx, double ty, double tz) {
	mat4_set_identity(A);
	mat4_set(A, 3, 0, tx);
	mat4_set(A, 3 ,1, ty);
	mat4_set(A, 3, 2, tz);
}
  
void mat4_homothety(mat4 A, double hx, double hy, double hz) {
	mat4_set_identity(A);
	mat4_set(A, 0, 0, hx);
	mat4_set(A, 1 ,1, hy);
	mat4_set(A, 2, 2, hz);
}
  
void mat4_rotation_x(mat4 A, double alpha) {
	mat4_set_identity(A);
	mat4_set(A, 1, 1, cos(alpha));
	mat4_set(A, 1, 2, sin(alpha));
	mat4_set(A, 2, 1, -sin(alpha));
	mat4_set(A, 2, 2, cos(alpha));
}
  
void mat4_rotation_y(mat4 A, double alpha) {
	mat4_set_identity(A);
	mat4_set(A, 0, 0, cos(alpha));
	mat4_set(A, 0, 2, -sin(alpha));
	mat4_set(A, 2, 0, sin(alpha));
	mat4_set(A, 2, 2, cos(alpha));
}
  
void mat4_rotation_z(mat4 A, double alpha) {
	mat4_set_identity(A);
	mat4_set(A, 0, 0, cos(alpha));
	mat4_set(A, 0, 1, sin(alpha));
	mat4_set(A, 1, 0, -sin(alpha));
	mat4_set(A, 1, 1, cos(alpha));
}

void mat4_product_mat4(mat4 C, mat4 A, mat4 B) {
	double x00 = mat4_get(A, 0, 0)*mat4_get(B, 0, 0) + mat4_get(A, 1, 0)*mat4_get(B, 0, 1) + mat4_get(A, 2, 0)*mat4_get(B, 0, 2) + mat4_get(A, 3, 0)*mat4_get(A, 0, 3);
	double x01 = mat4_get(A, 0, 0)*mat4_get(B, 1, 0) + mat4_get(A, 1, 0)*mat4_get(B, 1, 1) + mat4_get(A, 2, 0)*mat4_get(B, 1, 2) + mat4_get(A, 3, 0)*mat4_get(A, 1, 3);
	double x02 = mat4_get(A, 0, 0)*mat4_get(B, 2, 0) + mat4_get(A, 1, 0)*mat4_get(B, 2, 1) + mat4_get(A, 2, 0)*mat4_get(B, 2, 2) + mat4_get(A, 3, 0)*mat4_get(A, 2, 3);
	double x03 = mat4_get(A, 0, 0)*mat4_get(B, 3, 0) + mat4_get(A, 1, 0)*mat4_get(B, 3, 1) + mat4_get(A, 2, 0)*mat4_get(B, 3, 2) + mat4_get(A, 3, 0)*mat4_get(A, 3, 3);
	double x10 = mat4_get(A, 0, 1)*mat4_get(B, 0, 0) + mat4_get(A, 1, 1)*mat4_get(B, 0, 1) + mat4_get(A, 2, 1)*mat4_get(B, 0, 2) + mat4_get(A, 3, 1)*mat4_get(A, 0, 3);
	double x11 = mat4_get(A, 0, 1)*mat4_get(B, 1, 0) + mat4_get(A, 1, 1)*mat4_get(B, 1, 1) + mat4_get(A, 2, 1)*mat4_get(B, 1, 2) + mat4_get(A, 3, 1)*mat4_get(A, 1, 3);
	double x12 = mat4_get(A, 0, 1)*mat4_get(B, 2, 0) + mat4_get(A, 1, 1)*mat4_get(B, 2, 1) + mat4_get(A, 2, 1)*mat4_get(B, 2, 2) + mat4_get(A, 3, 1)*mat4_get(A, 2, 3);
	double x13 = mat4_get(A, 0, 1)*mat4_get(B, 3, 0) + mat4_get(A, 1, 1)*mat4_get(B, 3, 1) + mat4_get(A, 2, 1)*mat4_get(B, 3, 2) + mat4_get(A, 3, 1)*mat4_get(A, 3, 3);
	double x20 = mat4_get(A, 0, 2)*mat4_get(B, 0, 0) + mat4_get(A, 1, 2)*mat4_get(B, 0, 1) + mat4_get(A, 2, 2)*mat4_get(B, 0, 2) + mat4_get(A, 3, 2)*mat4_get(A, 0, 3);
	double x21 = mat4_get(A, 0, 2)*mat4_get(B, 1, 0) + mat4_get(A, 1, 2)*mat4_get(B, 1, 1) + mat4_get(A, 2, 2)*mat4_get(B, 1, 2) + mat4_get(A, 3, 2)*mat4_get(A, 1, 3);
	double x22 = mat4_get(A, 0, 2)*mat4_get(B, 2, 0) + mat4_get(A, 1, 2)*mat4_get(B, 2, 1) + mat4_get(A, 2, 2)*mat4_get(B, 2, 2) + mat4_get(A, 3, 2)*mat4_get(A, 2, 3);
	double x23 = mat4_get(A, 0, 2)*mat4_get(B, 3, 0) + mat4_get(A, 1, 2)*mat4_get(B, 3, 1) + mat4_get(A, 2, 2)*mat4_get(B, 3, 2) + mat4_get(A, 3, 2)*mat4_get(A, 3, 3);
	double x30 = mat4_get(A, 0, 3)*mat4_get(B, 0, 0) + mat4_get(A, 1, 3)*mat4_get(B, 0, 1) + mat4_get(A, 2, 3)*mat4_get(B, 0, 2) + mat4_get(A, 3, 3)*mat4_get(A, 0, 3);
	double x31 = mat4_get(A, 0, 3)*mat4_get(B, 1, 0) + mat4_get(A, 1, 3)*mat4_get(B, 1, 1) + mat4_get(A, 2, 3)*mat4_get(B, 1, 2) + mat4_get(A, 3, 3)*mat4_get(A, 1, 3);
	double x32 = mat4_get(A, 0, 3)*mat4_get(B, 2, 0) + mat4_get(A, 1, 3)*mat4_get(B, 2, 1) + mat4_get(A, 2, 3)*mat4_get(B, 2, 2) + mat4_get(A, 3, 3)*mat4_get(A, 2, 3);
	double x33 = mat4_get(A, 0, 3)*mat4_get(B, 3, 0) + mat4_get(A, 1, 3)*mat4_get(B, 3, 1) + mat4_get(A, 2, 3)*mat4_get(B, 3, 2) + mat4_get(A, 3, 3)*mat4_get(A, 3, 3);
	mat4_set(C, 0, 0, x00);
	mat4_set(C, 1, 0, x01);
	mat4_set(C, 2, 0, x02);
	mat4_set(C, 3, 0, x03);
	mat4_set(C, 0, 1, x10);
	mat4_set(C, 1, 1, x11);
	mat4_set(C, 2, 1, x12);
	mat4_set(C, 3, 1, x13);
	mat4_set(C, 0, 2, x20);
	mat4_set(C, 1, 2, x21);
	mat4_set(C, 2, 2, x22);
	mat4_set(C, 3, 2, x23);
	mat4_set(C, 0, 3, x30);
	mat4_set(C, 1, 3, x31);
	mat4_set(C, 2, 3, x32);
	mat4_set(C, 3, 3, x33);
}

void mat4_product_coord3(coord3 W, mat4 A, vec3 V) {
	double x = mat4_get(A, 0, 0)*vec3_get_x(V) + mat4_get(A, 1, 0)*vec3_get_y(V) + mat4_get(A, 2, 0)*vec3_get_z(V) + mat4_get(A, 3, 0);
	double y = mat4_get(A, 0, 1)*vec3_get_x(V) + mat4_get(A, 1, 1)*vec3_get_y(V) + mat4_get(A, 2, 1)*vec3_get_z(V) + mat4_get(A, 3, 1);
	double z = mat4_get(A, 0, 2)*vec3_get_x(V) + mat4_get(A, 1, 2)*vec3_get_y(V) + mat4_get(A, 2, 2)*vec3_get_z(V) + mat4_get(A, 3, 2);
	coord3_set(W, x, y, z);
}

void mat4_product_vec3(vec3 W, mat4 A, vec3 V) {
	mat4_product_coord3(W, A, V);
	vec3_normalize(W);
}

void mat4_product_point3(point3 Q, mat4 A, point3 P) {
	mat4_product_coord3(Q, A, P);
}
