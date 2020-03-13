#include "types.h"

#include <math.h>
#include <stdio.h>

void mat4_translation(mat4 A, double tx, double ty, double tz) {
	mat4_set_identity(A);
	A[a03] = tx;
	A[a13] = ty;
	A[a23] = tz;
}
  
void mat4_homothety(mat4 A, double hx, double hy, double hz) {
	mat4_set_identity(A);
	A[a00] = hx;
	A[a11] = hy;
	A[a22] = hz;
}
  
void mat4_rotation_x(mat4 A, double alpha) {
	mat4_set_identity(A);
	A[a11] = cos(alpha);
	A[a21] = sin(alpha);
	A[a12] = -sin(alpha);
	A[a22] = cos(alpha);
}
  
void mat4_rotation_y(mat4 A, double alpha) {
	mat4_set_identity(A);
	A[a00] = cos(alpha);
	A[a20] = -sin(alpha);
	A[a02] = sin(alpha);
	A[a22] = cos(alpha);
}
  
void mat4_rotation_z(mat4 A, double alpha) {
	mat4_set_identity(A);
	A[a00] = cos(alpha);
	A[a10] = sin(alpha);
	A[a01] = -sin(alpha);
	A[a11] = cos(alpha);
}

void mat4_product_mat4(mat4 A, mat4 B, mat4 C) {
	double x00 = A[a00]*B[a00] + A[a01]*B[a10] + A[a02]*B[a20] + A[a03]*B[a30];
	double x01 = A[a00]*B[a01] + A[a01]*B[a11] + A[a02]*B[a21] + A[a03]*B[a31];
	double x02 = A[a00]*B[a02] + A[a01]*B[a12] + A[a02]*B[a22] + A[a03]*B[a32];
	double x03 = A[a00]*B[a03] + A[a01]*B[a13] + A[a02]*B[a23] + A[a03]*B[a33];
	
	double x10 = A[a10]*B[a00] + A[a11]*B[a10] + A[a12]*B[a20] + A[a13]*B[a30];
	double x11 = A[a10]*B[a01] + A[a11]*B[a11] + A[a12]*B[a21] + A[a13]*B[a31];
	double x12 = A[a10]*B[a02] + A[a11]*B[a12] + A[a12]*B[a22] + A[a13]*B[a32];
	double x13 = A[a10]*B[a03] + A[a11]*B[a13] + A[a12]*B[a23] + A[a13]*B[a33];
	
	double x20 = A[a20]*B[a00] + A[a21]*B[a10] + A[a22]*B[a20] + A[a23]*B[a30];
	double x21 = A[a20]*B[a01] + A[a21]*B[a11] + A[a22]*B[a21] + A[a23]*B[a31];
	double x22 = A[a20]*B[a02] + A[a21]*B[a12] + A[a22]*B[a22] + A[a23]*B[a32];
	double x23 = A[a20]*B[a03] + A[a21]*B[a13] + A[a22]*B[a23] + A[a23]*B[a33];
	
	double x30 = A[a30]*B[a00] + A[a31]*B[a10] + A[a32]*B[a20] + A[a33]*B[a30];
	double x31 = A[a30]*B[a01] + A[a31]*B[a11] + A[a32]*B[a21] + A[a33]*B[a31];
	double x32 = A[a30]*B[a02] + A[a31]*B[a12] + A[a32]*B[a22] + A[a33]*B[a32];
	double x33 = A[a30]*B[a03] + A[a31]*B[a13] + A[a32]*B[a23] + A[a33]*B[a33];
	
	C[a00] = x00;
	C[a01] = x01;
	C[a02] = x02;
	C[a03] = x03;
	
	C[a10] = x10;
	C[a11] = x11;
	C[a12] = x12;
	C[a13] = x13;
	
	C[a20] = x20;
	C[a21] = x21;
	C[a22] = x22;
	C[a23] = x23;
	
	C[a30] = x30;
	C[a31] = x31;
	C[a32] = x32;
	C[a33] = x33;
	
}

void mat4_product_vec3(mat4 A, vec3 V, vec3 W) {
	double x = A[a00]*V[0] + A[a01]*V[1] + A[a02]*V[2] + A[a03];
	double y = A[a10]*V[0] + A[a11]*V[1] + A[a12]*V[2] + A[a13];
	double z = A[a20]*V[0] + A[a21]*V[1] + A[a22]*V[2] + A[a23];
	W[0] = x;
	W[1] = y;
	W[2] = z;
}

void mat4_product_point3(mat4 A, point3 P, point3 Q) {
	double x = A[a00]*P[0] + A[a01]*P[1] + A[a02]*P[2] + A[a03];
	double y = A[a10]*P[0] + A[a11]*P[1] + A[a12]*P[2] + A[a13];
	double z = A[a20]*P[0] + A[a21]*P[1] + A[a22]*P[2] + A[a23];
	Q[0] = x;
	Q[1] = y;
	Q[2] = z;
}
