#include "parser.h"

#include "types.h"
#include "shape.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LEN_NODE_TOKEN (30)
#define STR_MAX_LEN_NODE_TOKEN "30"
#define MAX_DOUBLE_DIGIT (20)
#define MAX_LEN_LINE (MAX_LEN_NODE_TOKEN +  13*MAX_DOUBLE_DIGIT + 22)

int _parser_number_line_;

static Tree parse_sphere(char *args) {
	assert(NULL != args);
	color4 color;
	double tx,ty,tz;
	double rx,ry,rz;
	double hx,hy,hz;
	int read = sscanf(args, "(%f,%f,%f,%f) (%lf,%lf,%lf) (%lf,%lf,%lf) (%lf,%lf,%lf)", color, color + 1, color + 2, color + 3, &tx, &ty, &tz, &rx, &ry, &rz, &hx, &hy, &hz); 
	if (read != 13) {
		fprintf(stderr, "line %d : invalid shape arguments\n", _parser_number_line_);
		exit(EXIT_FAILURE);
	} 
	Tree tree = tree_allocate_leaf(shape_sphere(color));
	tree_translation(tree,tx,ty,tz);
	tree_rotation(tree,rx,ry,rz);
	tree_homothety(tree,hx,hy,hz);
	return tree;
}

static Tree parse_cube(char *args) {
	assert(NULL != args);
	color4 color;
	double tx,ty,tz;
	double rx,ry,rz;
	double hx,hy,hz;
	int read = sscanf(args, "(%f,%f,%f,%f) (%lf,%lf,%lf) (%lf,%lf,%lf) (%lf,%lf,%lf)", color, color + 1, color + 2, color + 3, &tx, &ty, &tz, &rx, &ry, &rz, &hx, &hy, &hz); 
	if (read != 13) {
		fprintf(stderr, "line %d : invalid shape arguments\n", _parser_number_line_);
		exit(EXIT_FAILURE);
	} 
	Tree tree = tree_allocate_leaf(shape_cube(color));
	tree_translation(tree,tx,ty,tz);
	tree_rotation(tree,rx,ry,rz);
	tree_homothety(tree,hx,hy,hz);
	return tree;
}

static Tree parse_cylinder(char *args) {
	assert(NULL != args);
	color4 color;
	double tx,ty,tz;
	double rx,ry,rz;
	double hx,hy,hz;
	int read = sscanf(args, "(%f,%f,%f,%f) (%lf,%lf,%lf) (%lf,%lf,%lf) (%lf,%lf,%lf)", color, color + 1, color + 2, color + 3, &tx, &ty, &tz, &rx, &ry, &rz, &hx, &hy, &hz); 
	if (read != 13) {
		fprintf(stderr, "line %d : invalid shape arguments\n", _parser_number_line_);
		exit(EXIT_FAILURE);
	} 
	Tree tree = tree_allocate_leaf(shape_cylinder(color));
	tree_translation(tree,tx,ty,tz);
	tree_rotation(tree,rx,ry,rz);
	tree_homothety(tree,hx,hy,hz);
	return tree;
}

static Tree parse_cone(char *args) {
	assert(NULL != args);
	color4 color;
	double tx,ty,tz;
	double rx,ry,rz;
	double hx,hy,hz;
	int read = sscanf(args, "(%f,%f,%f,%f) (%lf,%lf,%lf) (%lf,%lf,%lf) (%lf,%lf,%lf)", color, color + 1, color + 2, color + 3, &tx, &ty, &tz, &rx, &ry, &rz, &hx, &hy, &hz); 
	if (read != 13) {
		fprintf(stderr, "line %d : invalid shape arguments\n", _parser_number_line_);
		exit(EXIT_FAILURE);
	} 
	Tree tree = tree_allocate_leaf(shape_cone(color));
	tree_translation(tree,tx,ty,tz);
	tree_rotation(tree,rx,ry,rz);
	tree_homothety(tree,hx,hy,hz);
	return tree;
}

static Tree parse_torus(char *args) {
	assert(NULL != args);
	double r;
	color4 color;
	double tx,ty,tz;
	double rx,ry,rz;
	double hx,hy,hz;
	int read = sscanf(args, "%lf (%f,%f,%f,%f) (%lf,%lf,%lf) (%lf,%lf,%lf) (%lf,%lf,%lf)", &r, color, color + 1, color + 2, color + 3, &tx, &ty, &tz, &rx, &ry, &rz, &hx, &hy, &hz); 
	if (read != 14 || r <= 0) {
		fprintf(stderr, "line %d : invalid shape arguments\n", _parser_number_line_);
		exit(EXIT_FAILURE);
	} 
	Tree tree = tree_allocate_leaf(shape_torus(color,r));
	tree_translation(tree,tx,ty,tz);
	tree_rotation(tree,rx,ry,rz);
	tree_homothety(tree,hx,hy,hz);
	return tree;
}

static Tree parse_leaf(char *strshape, char *args) {
	assert(NULL != strshape);
	assert(NULL != args);
	if (strncmp(strshape,SHAPE_SPHERE,MAX_LEN_NODE_TOKEN) == 0) {
		return parse_sphere(args);
	}
	if (strncmp(strshape,SHAPE_CUBE,MAX_LEN_NODE_TOKEN) == 0) {
		return parse_cube(args);
	}
	if (strncmp(strshape,SHAPE_CYLINDER,MAX_LEN_NODE_TOKEN) == 0) {
		return parse_cylinder(args);
	}
	if (strncmp(strshape,SHAPE_CONE,MAX_LEN_NODE_TOKEN) == 0) {
		return parse_cone(args);
	}
	if (strncmp(strshape,SHAPE_TORUS,MAX_LEN_NODE_TOKEN) == 0) {
		return parse_torus(args);
	}
	fprintf(stderr, "line %d : invalid token '%s'\n", _parser_number_line_, strshape);
	exit(EXIT_FAILURE);
}

Tree parse_tree_aux(FILE *file) {
	assert(NULL != file);
	Tree tree = NULL, left = NULL, right = NULL;
	char *line = NULL;
	char node[MAX_LEN_NODE_TOKEN];
	if(NULL == (line = (char *) malloc(MAX_LEN_LINE*sizeof(char)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)\n", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == fgets(line,MAX_LEN_LINE,file)) {
		fprintf(stderr, "line %d : unexpected end of file\n", _parser_number_line_);
		exit(EXIT_FAILURE);
	}
	int l = ++_parser_number_line_;
	sscanf(line, "%" STR_MAX_LEN_NODE_TOKEN "s",node);
	line += 1 + strlen(node);
	if (strncmp(node,OPERATOR_IDENTITY,MAX_LEN_NODE_TOKEN) == 0) {
		left = parse_tree_aux(file);
		right = parse_tree_aux(file);
		tree = tree_allocate_node(Identity, left, right);
	} else if (strncmp(node,OPERATOR_UNION,MAX_LEN_NODE_TOKEN) == 0) {
		left = parse_tree_aux(file);
		right = parse_tree_aux(file);
		tree = tree_allocate_node(Union, left, right);
	} else if (strncmp(node,OPERATOR_INTERSECTION,MAX_LEN_NODE_TOKEN) == 0) {
		left = parse_tree_aux(file);
		right = parse_tree_aux(file);
		tree = tree_allocate_node(Intersection, left, right);
	} else if (strncmp(node,OPERATOR_DIFFERENCE,MAX_LEN_NODE_TOKEN) == 0) {
		left = parse_tree_aux(file);
		right = parse_tree_aux(file);
		tree = tree_allocate_node(Difference, left, right);
	} else {
		return parse_leaf(node,line);
	}
	double tx,ty,tz;
	double rx,ry,rz;
	double hx,hy,hz;
	int read = sscanf(line, "(%lf,%lf,%lf) (%lf,%lf,%lf) (%lf,%lf,%lf)", &tx, &ty, &tz, &rx, &ry, &rz, &hx, &hy, &hz); 
	if (read != 9) {
		fprintf(stderr, "line %d : invalid operator arguments\n", l);
		exit(EXIT_FAILURE);
	} 
	tree_translation(tree,tx,ty,tz);
	tree_rotation(tree,rx,ry,rz);
	tree_homothety(tree,hx,hy,hz);
	return tree;
}

Tree parse_tree(FILE *file) {
	assert(NULL != file);
	_parser_number_line_ = 0;
	return parse_tree_aux(file); 
}
