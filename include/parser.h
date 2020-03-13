/**
 * \file parser.h
 * \brief CSG tree parsing module
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include "tree.h"
#include <stdio.h>

/**
 * \brief Sphere token in a scene file
 */ 
#define SHAPE_SPHERE ("sphere")

/**
 * \brief Cube token in a scene file
 */ 
#define SHAPE_CUBE ("cube")

/**
 * \brief Cylinder token in a scene file
 */ 
#define SHAPE_CYLINDER ("cylinder")

/**
 * \brief Cone token in a scene file
 */ 
#define SHAPE_CONE ("cone")

/**
 * \brief Tore token in a scene file
 */ 
#define SHAPE_TORUS ("torus")

/**
 * \brief Identity operator token in a scene file
 */ 
#define OPERATOR_IDENTITY ("=")

/**
 * \brief Union operator token in a scene file
 */ 
#define OPERATOR_UNION ("+")

/**
 * \brief Intersection operator token in a scene file
 */ 
#define OPERATOR_INTERSECTION ("*")

/**
 * \brief Difference operator token in a scene file
 */ 
#define OPERATOR_DIFFERENCE ("-")

/**
 * \brief Parse a file to convert it to a CSG tree
 * 
 * \details The file must be the result of the depth scan of the tree to be allocated.
 * One line in the file should correspond either to a canonical shape or to a combination operator.
 * Check the readme file for more information on the format to follow.
 * The format must be respected otherwise the program stops.
 * 
 * \param file CSG tree file to parse \n
 * Can not take the value \e NULL
 * 
 * \return the CSG tree represented by the file
 */  
Tree parse_tree(FILE *file);

#endif
