/**
 * \file point_cloud.h
 * \brief Point clound management module
 */
 
#ifndef __POINT_CLOUD__H__
#define __POINT_CLOUD__H__

#include "types.h"

/**
 * \brief Size of a point in the point cloud rendering
 */ 
#define POINT_SIZE (2)

/**
 * \brief Structure defining a point cloud
 * 
 * \details A point cloud is defined by a dynamic array of
 * dots, normals and colors.
 */ 
typedef struct {
	point3 *vrtx; /**< Points array */
	vec3 *norm; /**< Normals array */
	color4 *colors; /**< Colors array */
	int size; /**< Arrays size */
} PointCloud;

/**
 * \brief Allocate a point cloud
 * 
 * \details Arrays in parameters must absolutely be dynamically allocated.
 * They will be released when the point cloud is released.
 * The arrays should have the same size.
 * The program stops if the allocation has failed.
 * 
 * \param vrtx Points array \n
 * Can not take the value \e NULL
 * 
 * \param norm Normals array \n
 * Can not take the value \e NULL
 * 
 * \param colors Colors array \n
 * Can not take the value \e NULL
 * 
 * \param size Arrays size \n
 * Must be strictly positive
 * 
 * \return a pointer to the allocated point cloud
 */ 
PointCloud * point_cloud_allocate(point3 *vrtx, vec3 *norm, color4 *colors, int size);

/**
 * \brief Draw a point cloud
 * 
 * \param point_cloud Point cloud to draw \n
 * Can not take the value \e NULL \n
 * Must be a valid point cloud
 */ 
void point_cloud_draw(const PointCloud *point_cloud);

/**
 * \brief Free the memory allocated by a point cloud
 * 
 * \details Arrays will also be deallocated and the pointed point cloud will be set to \e NULL.
 *
 * \param point_cloud Pointer to the point cloud to free \n
 * Can not take the value \e NULL
 * Must be a valid point cloud
 */ 
void point_cloud_free (PointCloud **point_cloud);

/**
 * \brief Test if a point cloud datastructure is valid
 * 
 * \details A point cloud is valid if : \n
 * - \b size is strictly positive \n
 * - \b vrtx is different from \e NULL \n
 * - \b norm is different from \e NULL \n
 * - \b colors is different from \e NULL
 * 
 * \param point_cloud Point cloud to test \n
 * Can not take the value \e NULL
 * 
 * \return \e 1 if the point cloud is valid, \e 0 otherwise
 */ 
int point_cloud_is_valid(const PointCloud *point_cloud);

#endif
