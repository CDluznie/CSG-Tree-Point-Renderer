/**
 * \file tree.h
 * \brief CSG tree management module
 */

#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"
#include "point_cloud.h"
#include "shape.h"

/**
 * \brief Enumeration of the different combination operations available
 */ 
typedef enum {
	Union, /**< Union */
	Intersection, /**< Intersection */
	Difference, /**< Difference */
	Identity, /**< Identity (union with intersection kept) */
	NumberOperator /**< Number of combination operator */
} Operator;

/*
 * TODO add explanation on Union - Identity
 * maybe rename Union -> Symetric difference
 * 			    Identity -> Union
 */

/**
 * \brief Structure defining a CSG tree
 * 
 * \details A CSG tree is a recursive structure.
 * If the tree is a leaf, it is defined by a canonical form.
 * If the tree is an internal node, it is defined by a combination operator, a left CSG tree child and a right CSG tree child.
 * In both cases, the tree also contains a points transformation matrix, an inverse points transformation matrix, and a normals transformation matrix.
 */ 
typedef struct Node {
	Operator op; /**< Combination operator */
	Shape* shape; /**< Canonical shape */
	mat4 transformations; /**< Points transformation matrix */
	mat4 inv_transformations; /**< Inverse points transformation matrix */
	mat4 norm_transformations; /**< Normals transformation matrix */
	struct Node *left; /**< Left CSG subtree */
	struct Node *right; /**< Right CSG subtree */
} *Tree;

/**
 * \brief Allocate a leaf of a CSG tree
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e tree_free
 *
 * \param shape Canonical shape \n
 * Can not take the value \e NULL \n
 * Must be a valid canonical shape
 * 
 * \return a pointer to the allocated leaf
 */ 
Tree tree_allocate_leaf (Shape * shape);

/**
 * \brief Allocate an internal node of a CSG tree
 * 
 * \details The program stops if the allocation has failed.
 * This function allocate some memory that need to be freed with \e tree_free.
 * 
 * \param op Combination operator at the root of the tree
 * 
 * \param left Left subtree \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
 * 
 * \param right Right subtree \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
 * 
 * \return a pointer to the allocated internal node
 */ 
Tree tree_allocate_node (Operator op, Tree left, Tree right);

/**
 * \brief Free the memory allocated by a CSG tree
 * 
 * \details The pointed CSG tree will be set to \e NULL.
 *
 * \param shape Pointer to the CSG tree to free \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
 */ 
void tree_free (Tree *tree);

/**
 * \brief Convert a CSG tree to a point cloud
 *
 * \details This function is necessary in order to be able to the render a CSG tree.
 * This function allocate some memory that need to be freed with \e point_cloud_free.
 *
 * \param shape CSG tree to convert \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
 *
 * \param density Point density per unit area \n
 * Must be strictly positive
 *
 * \return a pointer to the allocated point cloud
 */
PointCloud * tree_to_point_cloud(Tree tree, int density);

/**
 * \brief Perform an homothety on a CSG tree
 * 
 * \details Operation will update matrix fields of the tree.
 * Scaling factors of canonical shapes at leaves are also updated.
 * 
 * \param shape CSG tree to modify \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
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
void tree_homothety (Tree tree, double x, double y, double z);

/**
 * \brief Test if a CSG tree datastructure is valid
 * 
 * \details A CSG tree is valid if : \n
 * - \b tree is a leaf and \b shape is valid \n
 * - \b tree is internal node and \b op is between \e 0 and \e NumberOperator-1 \n
 * - \b tree is internal node and \b left is different from \e NULL \n
 * - \b tree is internal node and \b right is different from \e NULL
 * 
 * \param tree CSG tree to test \n
 * Can not take the value \e NULL \n
 * 
 * \return \e 1 if CSG tree is valid, \e 0 otherwise
 */ 
int tree_is_valid(Tree tree);

/**
 * \brief Perform a rotation on a CSG tree
 * 
 * \details Operation will update matrix fields of the tree.
 * 
 * \param shape CSG tree to modify \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
 * 
 * \param x Rotation angle around \e x axis
 * 
 * \param y Rotation angle around \e y axis
 * 
 * \param z Rotation angle around \e z axis
 */ 
void tree_rotation (Tree tree, double x, double y, double z);

/**
 * \brief Perform a translation on a CSG tree
 * 
 * \details Operation will update matrix fields of the tree.
 * 
 * \param shape CSG tree to modify \n
 * Can not take the value \e NULL \n
 * Must be a valid CSG tree
 * 
 * \param x Translation factor on \e x axis
 * 
 * \param y Translation factor on \e y axis
 * 
 * \param z Translation factor on \e z axis
 */ 
void tree_translation (Tree tree, double x, double y, double z);
 
#endif
