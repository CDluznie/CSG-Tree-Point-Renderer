/**
 * \file types.h
 * \brief basic operations management module
 */
 
#ifndef __TYPES__H__
#define __TYPES__H__

#include <math.h>
#include <string.h>

/**
 * \brief Value of pi
 */
#define PI 3.1415926535897932384626433832795

/**
 * \brief Macro to compute square value
 */
#define SQUARE(a) ((a)*(a))

/**
 * \brief Macro to compute the minimum of 3 numbers
 */
#define MIN3(a,b,c) (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c)))

/**
 * \brief Macro to compute the maximum of 3 numbers
 */
#define MAX3(a,b,c) (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c)))

/**
 * \brief Structure defining a color
 *
 * \details A color is a RGBA array.
 */
typedef float color4[4];

/**
 * \brief Structure defining a 3D coordinate
 *
 * \details A 3D coordinate is a X,Y,Z array.
 */
typedef double coord3[3];

/**
 * \brief Structure defining a 3D point
 *
 * \details A 3D point is a 3D coordinate.
 */
typedef coord3 point3;

/**
 * \brief Structure defining a 3D vector
 *
 * \details A 3D vector is a 3D coordinate.
 */
typedef coord3 vec3;

/**
 * \brief Structure defining a 3D homogeneous matrix
 *
 * \details A 3D homogeneous matrix is a 1D plain array
 * containing the values of the matrix in a column-major order.
 */
typedef double mat4[16];

/**
 * \brief Set the values of a color
 *
 * \param color Color to modify
 *
 * \param r Red value of color
 *
 * \param r Green value of color
 *
 * \param r Blue value of color
 *
 * \param a Alpha value of color
 */
#define color4_set(color, r, g, b, a) ((color)[0]=(r),\
                            	   (color)[1]=(g),\
								   (color)[2]=(b),\
								   (color)[3]=(a))

/**
 * \brief Copy a color
 *
 * \param color1 Color to modify
 *
 * \param color2 Color to copy
 */
#define color4_copy(color1, color2) ((color1)[0]=(color2)[0],\
                            		 (color1)[1]=(color2)[1],\
									 (color1)[2]=(color2)[2],\
									 (color1)[3]=(color2)[3])

/**
 * \brief Scale a color
 *
 * \param color Color to modify
 *
 * \param s Scaling factor
 *
 * \return a scaled color
 */
#define color4_scale(color, s) {(color)[0]*s,\
                               (color)[1]*s,\
                               (color)[2]*s,\
                               (color)[3]*s}

/**
 * \brief Get the \e x value of the coordinate
 *
 * \param c Coordinate
 *
 * \return the \e x value of the coordinate
 */
#define coord3_get_x(c) ((c)[0])

/**
 * \brief Get the \e y value of the coordinate
 *
 * \param c Coordinate
 *
 * \return the \e y value of the coordinate
 */
#define coord3_get_y(c) ((c)[1])

/**
 * \brief Get the \e z value of the coordinate
 *
 * \param c Coordinate
 *
 * \return the \e z value of the coordinate
 */
#define coord3_get_z(c) ((c)[2])

/**
 * \brief Set the values of a coordinate
 *
 * \param v Coordinate to modify
 *
 * \param x Value of the coordinate on \e x axis
 *
 * \param y Value of the coordinate on \e y axis
 *
 * \param z Value of the coordinate on \e z axis
 */
#define coord3_set(c, x, y, z) ((c)[0]=(x),\
                            	(c)[1]=(y),\
								(c)[2]=(z))

/**
 * \brief Copy a coordinate
 *
 * \param c1 Coordinate to modify
 *
 * \param c2 Coordinate to copy
 */
#define coord3_copy(c1, c2) ((c1)[0]=(c2)[0],\
                             (c1)[1]=(c2)[1],\
							 (c1)[2]=(c2)[2])

/**
 * \brief Scale a coordinate
 *
 * \param color Coordinate to modify
 *
 * \param s Scaling factor
 *
 * \return a scaled coordinate
 */
#define coord3_scale(c, s) {(c)[0]*s,\
                         	(c)[1]*s,\
							(c)[2]*s}

/**
 * \brief Get the \e x value of the point
 *
 * \param p Point
 *
 * \return the \e x value of the point
 */
#define point3_get_x(p) (coord3_get_x(p))

/**
 * \brief Get the \e y value of the point
 *
 * \param p Point
 *
 * \return the \e y value of the point
 */
#define point3_get_y(p) (coord3_get_y(p))

/**
 * \brief Get the \e z value of the point
 *
 * \param p Point
 *
 * \return the \e z value of the point
 */
#define point3_get_z(p) (coord3_get_z(p))

/**
 * \brief Copy a point
 *
 * \param p1 Point to modify
 *
 * \param p2 Point to copy
 */
#define point3_copy(p1, p2) (coord3_copy(p1, p2))

/**
 * \brief Set the values of a point
 *
 * \param p point to modify
 *
 * \param x Value of the coordinate on \e x axis
 *
 * \param y Value of the coordinate on \e y axis
 *
 * \param z Value of the coordinate on \e z axis
 */
#define point3_set(p, x, y, z) (coord3_set(p, x, y, z))

/**
 * \brief Compute the vector between two points
 *
 * \param p1 The \e from point
 *
 * \param p2 The \e to point
 *
 * \return the vector going from p1 to p2
 */
#define point3_from_to_vector(p1, p2) {(p2)[0] - (p1)[0],\
									   (p2)[1] - (p1)[1],\
									   (p2)[2] - (p1)[2]}

/**
 * \brief Get the \e x value of the vector
 *
 * \param v Vector
 *
 * \return the \e x value of the vector
 */
#define vec3_get_x(v) (coord3_get_x(v))

/**
 * \brief Get the \e y value of the vector
 *
 * \param v Vector
 *
 * \return the \e y value of the vector
 */
#define vec3_get_y(v) (coord3_get_y(v))

/**
 * \brief Get the \e z value of the vector
 *
 * \param v Vector
 *
 * \return the \e z value of the vector
 */
#define vec3_get_z(v) (coord3_get_z(v))

/**
 * \brief Copy a vector
 *
 * \param v1 Point to modify
 *
 * \param v2 Point to copy
 */
#define vec3_copy(v1, v2) (coord3_copy(v1, v2))

/**
 * \brief Set the values of a vector
 *
 * \param v Vector to modify
 *
 * \param x Value of the vector on \e x axis
 *
 * \param y Value of the vector on \e y axis
 *
 * \param z Value of the vector on \e z axis
 */
#define vec3_set(v, x, y, z) (coord3_set(v, x, y, z))

/**
 * \brief Scale a vector
 *
 * \param v Vector to modify
 *
 * \param s Scaling factor
 *
 * \return a scaled vector
 */
#define vec3_scale(v, s) (coord3_scale(v, s))

/**
 * \brief Compute the dot product of vectors
 *
 * \param u Vector
 *
 * \param v Vector
 *
 * \return the dot product
 */
#define vec3_dot(u, v) ((u)[0]*(v)[0]+(u)[1]*(v)[1]+(u)[2]*(v)[2])

/**
 * \brief Compute the cross product of vectors
 *
 * \param u Vector
 *
 * \param v Vector
 *
 * \return the cross product
 */
#define vec3_cross(result, u, v) ((result)[0]=(u)[1]*(v)[2]-(u)[2]*(v)[1],\
                                  (result)[1]=(u)[2]*(v)[0]-(u)[0]*(v)[2],\
								  (result)[2]=(u)[0]*(v)[1]-(u)[1]*(v)[0])

/**
 * \brief Compute the norm of the vector
 *
 * \param v Vector
 *
 * \return the norm of the vector
 */
#define vec3_norm(v) (sqrt(vec3_dot(v,v)))

/**
 * \brief Compute a product between a homogeneous matrix and a point
 *
 * \details The vector is normalized in place.
 *
 * \param v Vector to normalize
 */
void vec3_normalize(vec3 v);

/**
 * \brief Set a value in the homogeneous matrix
 *
 * \param m Homogeneous matrix to modify
 *
 * \param col Column index
 *
 * \param col Line index
 *
 * \param val Value to set
 */
#define mat4_set(m, col, line, val) ((m)[4*(col)+(line)]=(val))

/**
 * \brief Get a value in the homogeneous matrix
 *
 * \param m Homogeneous matrix
 *
 * \param col Column index
 *
 * \param col Line index
 *
 * \return value of the homogeneous matrix at given indices
 */
#define mat4_get(m, col, line) ((m)[4*(col)+(line)])

/**
 * \brief The indentiy homogeneous matrix
 */
#define mat4_identity {1.,0.,0.,0.,\
                       0.,1.,0.,0.,\
                       0.,0.,1.,0.,\
                       0.,0.,0.,1.}

/**
 * \brief Set the identity matrix in the homogeneous matrix
 *
 * \param m Homogeneous matrix to modify
 */
#define mat4_set_identity(m) (memset((m), 0, sizeof(mat4)),\
                              (m)[0]=(m)[5]=(m)[10]=(m)[15]=1)

/**
 * \brief Compute a translation matrix
 *
 * \param m Homogeneous matrix to modify
 *
 * \param tx Translation factor on \e x axis
 *
 * \param ty Translation factor on \e y axis
 *
 * \param tz Translation factor on \e z axis
 */
void mat4_translation(mat4 m, double tx, double ty, double tz);

/**
 * \brief Compute a homothety matrix
 *
 * \param m Homogeneous matrix to modify
 *
 * \param hx Scaling factor on \e x axis
 *
 * \param hy Scaling factor on \e y axis
 *
 * \param hz Scaling factor on \e z axis
 */
void mat4_homothety(mat4 m, double hx, double hy, double hz);

/**
 * \brief Compute a rotation matrix around x axis
 *
 * \param m Homogeneous matrix to modify
 *
 * \param alpha Rotation angle around \e x axis
 */
void mat4_rotation_x(mat4 m, double alpha);

/**
 * \brief Compute a rotation matrix around y axis
 *
 * \param m Homogeneous matrix to modify
 *
 * \param alpha Rotation angle around \e y axis
 */
void mat4_rotation_y(mat4 m, double alpha);

/**
 * \brief Compute a rotation matrix around z axis
 *
 * \param m Homogeneous matrix to modify
 *
 * \param alpha Rotation angle around \e z axis
 */
void mat4_rotation_z(mat4 m, double alpha);

/**
 * \brief Compute a product between homogeneous matrices
 *
 * \param m_result Homogeneous matrix to save the result
 *
 * \param m1 Left matrix of the product
 *
 * \param m2 Right matrix of the product
 */
void mat4_product_mat4(mat4 m_result, mat4 m1, mat4 m2);

/**
 * \brief Compute a product between a homogeneous matrix and a vector
 *
 * \param v_result Vector to save the result
 *
 * \param m Matrix of the product
 *
 * \param v Vector of the product
 */
void mat4_product_vec3(vec3 v_result, mat4 m, vec3 v);

/**
 * \brief Compute a product between a homogeneous matrix and a point
 *
 * \param p_result Point to save the result
 *
 * \param m Matrix of the product
 *
 * \param p Point of the product
 */
void mat4_product_point3(point3 p_result, mat4 m, point3 p);

#endif
