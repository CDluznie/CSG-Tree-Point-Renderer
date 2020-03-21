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

void mat4_translation(mat4 m, double tx, double ty, double tz) {
	mat4_set_identity(m);
	mat4_set(m, 3, 0, tx);
	mat4_set(m, 3 ,1, ty);
	mat4_set(m, 3, 2, tz);
}
  
void mat4_homothety(mat4 m, double hx, double hy, double hz) {
	mat4_set_identity(m);
	mat4_set(m, 0, 0, hx);
	mat4_set(m, 1 ,1, hy);
	mat4_set(m, 2, 2, hz);
}
  
void mat4_rotation_x(mat4 m, double alpha) {
	mat4_set_identity(m);
	mat4_set(m, 1, 1, cos(alpha));
	mat4_set(m, 1, 2, sin(alpha));
	mat4_set(m, 2, 1, -sin(alpha));
	mat4_set(m, 2, 2, cos(alpha));
}
  
void mat4_rotation_y(mat4 m, double alpha) {
	mat4_set_identity(m);
	mat4_set(m, 0, 0, cos(alpha));
	mat4_set(m, 0, 2, -sin(alpha));
	mat4_set(m, 2, 0, sin(alpha));
	mat4_set(m, 2, 2, cos(alpha));
}
  
void mat4_rotation_z(mat4 m, double alpha) {
	mat4_set_identity(m);
	mat4_set(m, 0, 0, cos(alpha));
	mat4_set(m, 0, 1, sin(alpha));
	mat4_set(m, 1, 0, -sin(alpha));
	mat4_set(m, 1, 1, cos(alpha));
}

void mat4_product_mat4(mat4 m_result, mat4 m1, mat4 m2) {
	double x00 = mat4_get(m1, 0, 0)*mat4_get(m2, 0, 0) + mat4_get(m1, 1, 0)*mat4_get(m2, 0, 1) + mat4_get(m1, 2, 0)*mat4_get(m2, 0, 2) + mat4_get(m1, 3, 0)*mat4_get(m1, 0, 3);
	double x01 = mat4_get(m1, 0, 0)*mat4_get(m2, 1, 0) + mat4_get(m1, 1, 0)*mat4_get(m2, 1, 1) + mat4_get(m1, 2, 0)*mat4_get(m2, 1, 2) + mat4_get(m1, 3, 0)*mat4_get(m1, 1, 3);
	double x02 = mat4_get(m1, 0, 0)*mat4_get(m2, 2, 0) + mat4_get(m1, 1, 0)*mat4_get(m2, 2, 1) + mat4_get(m1, 2, 0)*mat4_get(m2, 2, 2) + mat4_get(m1, 3, 0)*mat4_get(m1, 2, 3);
	double x03 = mat4_get(m1, 0, 0)*mat4_get(m2, 3, 0) + mat4_get(m1, 1, 0)*mat4_get(m2, 3, 1) + mat4_get(m1, 2, 0)*mat4_get(m2, 3, 2) + mat4_get(m1, 3, 0)*mat4_get(m1, 3, 3);
	double x10 = mat4_get(m1, 0, 1)*mat4_get(m2, 0, 0) + mat4_get(m1, 1, 1)*mat4_get(m2, 0, 1) + mat4_get(m1, 2, 1)*mat4_get(m2, 0, 2) + mat4_get(m1, 3, 1)*mat4_get(m1, 0, 3);
	double x11 = mat4_get(m1, 0, 1)*mat4_get(m2, 1, 0) + mat4_get(m1, 1, 1)*mat4_get(m2, 1, 1) + mat4_get(m1, 2, 1)*mat4_get(m2, 1, 2) + mat4_get(m1, 3, 1)*mat4_get(m1, 1, 3);
	double x12 = mat4_get(m1, 0, 1)*mat4_get(m2, 2, 0) + mat4_get(m1, 1, 1)*mat4_get(m2, 2, 1) + mat4_get(m1, 2, 1)*mat4_get(m2, 2, 2) + mat4_get(m1, 3, 1)*mat4_get(m1, 2, 3);
	double x13 = mat4_get(m1, 0, 1)*mat4_get(m2, 3, 0) + mat4_get(m1, 1, 1)*mat4_get(m2, 3, 1) + mat4_get(m1, 2, 1)*mat4_get(m2, 3, 2) + mat4_get(m1, 3, 1)*mat4_get(m1, 3, 3);
	double x20 = mat4_get(m1, 0, 2)*mat4_get(m2, 0, 0) + mat4_get(m1, 1, 2)*mat4_get(m2, 0, 1) + mat4_get(m1, 2, 2)*mat4_get(m2, 0, 2) + mat4_get(m1, 3, 2)*mat4_get(m1, 0, 3);
	double x21 = mat4_get(m1, 0, 2)*mat4_get(m2, 1, 0) + mat4_get(m1, 1, 2)*mat4_get(m2, 1, 1) + mat4_get(m1, 2, 2)*mat4_get(m2, 1, 2) + mat4_get(m1, 3, 2)*mat4_get(m1, 1, 3);
	double x22 = mat4_get(m1, 0, 2)*mat4_get(m2, 2, 0) + mat4_get(m1, 1, 2)*mat4_get(m2, 2, 1) + mat4_get(m1, 2, 2)*mat4_get(m2, 2, 2) + mat4_get(m1, 3, 2)*mat4_get(m1, 2, 3);
	double x23 = mat4_get(m1, 0, 2)*mat4_get(m2, 3, 0) + mat4_get(m1, 1, 2)*mat4_get(m2, 3, 1) + mat4_get(m1, 2, 2)*mat4_get(m2, 3, 2) + mat4_get(m1, 3, 2)*mat4_get(m1, 3, 3);
	double x30 = mat4_get(m1, 0, 3)*mat4_get(m2, 0, 0) + mat4_get(m1, 1, 3)*mat4_get(m2, 0, 1) + mat4_get(m1, 2, 3)*mat4_get(m2, 0, 2) + mat4_get(m1, 3, 3)*mat4_get(m1, 0, 3);
	double x31 = mat4_get(m1, 0, 3)*mat4_get(m2, 1, 0) + mat4_get(m1, 1, 3)*mat4_get(m2, 1, 1) + mat4_get(m1, 2, 3)*mat4_get(m2, 1, 2) + mat4_get(m1, 3, 3)*mat4_get(m1, 1, 3);
	double x32 = mat4_get(m1, 0, 3)*mat4_get(m2, 2, 0) + mat4_get(m1, 1, 3)*mat4_get(m2, 2, 1) + mat4_get(m1, 2, 3)*mat4_get(m2, 2, 2) + mat4_get(m1, 3, 3)*mat4_get(m1, 2, 3);
	double x33 = mat4_get(m1, 0, 3)*mat4_get(m2, 3, 0) + mat4_get(m1, 1, 3)*mat4_get(m2, 3, 1) + mat4_get(m1, 2, 3)*mat4_get(m2, 3, 2) + mat4_get(m1, 3, 3)*mat4_get(m1, 3, 3);
	mat4_set(m_result, 0, 0, x00);
	mat4_set(m_result, 1, 0, x01);
	mat4_set(m_result, 2, 0, x02);
	mat4_set(m_result, 3, 0, x03);
	mat4_set(m_result, 0, 1, x10);
	mat4_set(m_result, 1, 1, x11);
	mat4_set(m_result, 2, 1, x12);
	mat4_set(m_result, 3, 1, x13);
	mat4_set(m_result, 0, 2, x20);
	mat4_set(m_result, 1, 2, x21);
	mat4_set(m_result, 2, 2, x22);
	mat4_set(m_result, 3, 2, x23);
	mat4_set(m_result, 0, 3, x30);
	mat4_set(m_result, 1, 3, x31);
	mat4_set(m_result, 2, 3, x32);
	mat4_set(m_result, 3, 3, x33);
}

void mat4_product_vec3(vec3 v_result, mat4 m, vec3 v) {
	double x = mat4_get(m, 0, 0)*coord3_get_x(v) + mat4_get(m, 1, 0)*coord3_get_y(v) + mat4_get(m, 2, 0)*coord3_get_z(v);
	double y = mat4_get(m, 0, 1)*coord3_get_x(v) + mat4_get(m, 1, 1)*coord3_get_y(v) + mat4_get(m, 2, 1)*coord3_get_z(v);
	double z = mat4_get(m, 0, 2)*coord3_get_x(v) + mat4_get(m, 1, 2)*coord3_get_y(v) + mat4_get(m, 2, 2)*coord3_get_z(v);
	vec3_set(v_result, x, y, z);
	vec3_normalize(v_result);
}

void mat4_product_point3(point3 p_result, mat4 m, point3 p) {
	double x = mat4_get(m, 0, 0)*coord3_get_x(p) + mat4_get(m, 1, 0)*coord3_get_y(p) + mat4_get(m, 2, 0)*coord3_get_z(p) + mat4_get(m, 3, 0);
	double y = mat4_get(m, 0, 1)*coord3_get_x(p) + mat4_get(m, 1, 1)*coord3_get_y(p) + mat4_get(m, 2, 1)*coord3_get_z(p) + mat4_get(m, 3, 1);
	double z = mat4_get(m, 0, 2)*coord3_get_x(p) + mat4_get(m, 1, 2)*coord3_get_y(p) + mat4_get(m, 2, 2)*coord3_get_z(p) + mat4_get(m, 3, 2);
	point3_set(p_result, x, y, z);
}
