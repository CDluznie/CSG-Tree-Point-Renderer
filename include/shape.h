/**
 * \file shape.h
 * \brief Canonical shapes management module
 */

#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "types.h"
#include "point_cloud.h"

/**
 * \brief Enumeration of the different types of canonical shapes available
 * 
 * \details This information is not essential but can be useful to make statistics on the CSG tree,
 * like for example to count the number of cube in the scene.
 */ 
typedef enum {
	Sphere, /**< Canonical sphere */
	Cube, /**< Canonical cube */
	Cylinder, /**< Canonical cylinder */
	Cone, /**< Canonical cone */
	Torus, /**< Canonical torus */
	NumberShapeType /**< Number of types of canonical shapes */
} ShapeType;

/**
 * \brief Structure defining a canonical shape
 * 
 * \details Un objet canonique est définit par un type de forme, une couleur,
 * scaling factors, real number arguments (none or several) for parametric shape like torus,
 * a point belonging function, and a conversion function to a point cloud.
 */ 
typedef struct {
	ShapeType type; /**< Canonical shape type */
	color4 color; /**< Color */
	double x_scale; /**< Scaling factor on \e x axis */
	double y_scale; /**< Scaling factor on \e y axis */
	double z_scale; /**< Scaling factor on \e z axis */
	double *args; /**< real arguments (for parametric shapes) */
	int (*contains_function)(double *, point3 *); /**< function pointer on the point belonging function */
	PointCloud * (*point_cloud_converter)(int, color4, mat4, mat4, double, double, double, double *); /**< function pointer on the point cloud conversion function */
} Shape;

/**
 * \brief Convert a canonical shape to a point cloud
 * 
 * \details This function is necessary in order to be able to the render a canonical shape.
 * This function allocate some memory that need to be freed with \e point_cloud_free.
 * 
 * \param shape Canonical shape to convert \n
 * Can not take the value \e NULL \n
 * Must be a valid canonical shape
 * 
 * \param density Point density per unit area \n
 * Must be strictly positive
 * 
 * \param transformations Points transformation matrix
 * 
 * \param norm_transformations Normals transformation matrix
 * 
 * \return a pointer to the allocated point cloud
 */ 
PointCloud * shape_to_point_cloud(const Shape *shape, int density, mat4 transformations, mat4 norm_transformations);

/**
 * \brief Allocate a canonical cone
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e shape_free.
 * 
 * \param color Color of the canonical cone
 * 
 * \return a pointer to the allocated canonical cone
 */ 
Shape * shape_cone(color4 color);

/**
 * \brief Allocate a canonical cube
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e shape_free.
 *
 * \param color Color of the canonical cube
 * 
 * \return a pointer to the allocated canonical cube
 */ 
Shape * shape_cube(color4 color);

/**
 * \brief Test if a point belongs to a canonical shape
 *
 * \param shape Canonical shape to test \n
 * Can not take the value \e NULL \n
 * Must be a valid canonical shape
 * 
 * \param point Point à tester \n
 * Can not take the value \e NULL
 *
 * \return \e 1 if the point belongs to the canonical shape, \e 0 otherwise
 */ 
int shape_contains_point(const Shape *shape, const point3 *point);

/**
 * \brief Allocate a canonical cylinder
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e shape_free.
 *
 * \param color Color of the canonical cylinder
 * 
 * \return a pointer to the allocated canonical cylinder
 */ 
Shape * shape_cylinder(color4 color);

/**
 * \brief Free the memory allocated by a canonical shape
 * 
 * \details The pointed canonical shape will be set to \e NULL.
 *
 * \param shape Pointer to the canonical shape to free \n
 * Can not take the value \e NULL \n
 * Must be a valid canonical shape
 */ 
void shape_free(Shape **shape);

/**
 * \brief Test if a canonical shape datastructure is valid
 * 
 * \details A canonical shape is valid if : \n
 * - \b type is between \e 0 and \e NumberShapeType-1 \n
 * - \b contains_function is different from \e NULL \n
 * - \b point_cloud_converter is different from \e NULL \n
 * 
 * \param shape Pointer to the canonical shape to test \n
 * Can not take the value \e NULL
 * 
 * \return \e 1 if the canonical shape is valid, \e 0 otherwise
 */ 
int shape_is_valid(const Shape *shape);

/**
 * \brief Perform a scaling on a canonical shape
 * 
 * \details This function is important to estimate the volume of a canonical shape for the conversion to point cloud.
 * If on a canonical object we do two scaling on an axis,
 * the scale factor will correspond to the product of the two factors.
 * 
 * \param shape Canonical shape to scale \n
 * Can not take the value \e NULL \n
 * Must be a valid canonical shape
 * 
 * \param x Scaling factor on \e x axis \n
 * Must be strictly positive
 * 
 * \param y Scaling factor on \e y axis \n
 * Must be strictly positive
 * 
 * \param z Scaling factor on \e z axis \n
 * Must be strictly positive
 */ 
void shape_rescale(Shape *shape, double x, double y, double z);

/**
 * \brief Allocate a canonical sphere
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e shape_free.
 *
 * \param color Color of the canonical sphere
 * 
 * \return a pointer to the allocated canonical sphere
 */ 
Shape * shape_sphere(color4 color);

/**
 * \brief Allocate a canonical torus
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e shape_free.
 *
 * \param color Color of the canonical torus
 * 
 * \return a pointer to the allocated canonical torus
 */ 
Shape * shape_torus(color4 color, double radius);

#endif
