#include "tree.h"

#include "types.h"
#include "shape.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "point_cloud.h"


static Tree tree_allocate(Shape * shape, Operator op, Tree left, Tree right) {
	Tree t = NULL;
	if(NULL == (t = (Tree) malloc(sizeof(struct Node)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	t->shape = shape;
	t->left = left;
	t->right = right;
	t->op = op;
	mat4_set_identity(t->transformations);
	mat4_set_identity(t->inv_transformations);
	mat4_set_identity(t->norm_transformations);
	return t;
}

int tree_is_valid(Tree tree) {
	assert(NULL != tree);
	if (tree->shape != NULL)
		return shape_is_valid(tree->shape);
	if (tree->op < 0 || tree->op >= NumberOperator)
		return 0;
	if (tree->left == NULL || tree->right == NULL)
		return 0;
	return 1;
}

Tree tree_allocate_leaf (Shape * shape) {
	assert(NULL != shape);
	assert(shape_is_valid(shape));
	return tree_allocate(shape, 0, NULL, NULL);
}

Tree tree_allocate_node (Operator op, Tree left, Tree right) {
	assert(0 <= op && op < NumberOperator);
	assert(NULL != left); 
	assert(tree_is_valid(left)); 
	assert(NULL != right); 
	assert(tree_is_valid(right)); 
	return tree_allocate(NULL, op, left, right);
}

static void tree_transform(Tree tree, mat4 transformation, mat4 inv_transformation, mat4 norm_transformation) {
	assert(NULL != tree);
	assert(tree_is_valid(tree));
	mat4_product_mat4(tree->transformations, tree->transformations, transformation);
	mat4_product_mat4(tree->inv_transformations, inv_transformation, tree->inv_transformations);
	mat4_product_mat4(tree->norm_transformations, tree->norm_transformations, norm_transformation);
}  

void tree_translation(Tree tree, double x, double y, double z) {
	assert(NULL != tree);
	assert(tree_is_valid(tree));
	mat4 translation, inv_translation;
	mat4_translation(translation, x, y, z);
	mat4_translation(inv_translation, -x, -y, -z);
	tree_transform(tree, translation, inv_translation, translation);
}

static void rescale_node(Tree tree, double x, double y, double z) {
	assert(NULL != tree);
	assert(tree_is_valid(tree));
	assert(x > 0);
	assert(y > 0);
	assert(z > 0);
	if (NULL != tree->shape) {
		shape_rescale(tree->shape,x,y,z);
	} else {
		rescale_node(tree->left, x, y, z);
		rescale_node(tree->right, x, y, z);
	}	
}

void tree_homothety (Tree tree, double x, double y, double z) {
	assert(NULL != tree);
	assert(tree_is_valid(tree));
	assert(x > 0);
	assert(y > 0);
	assert(z > 0);
	mat4 homothetie, inv_homothetie, norm_homothetie;
	mat4_homothety(homothetie, x, y, z);
	mat4_homothety(inv_homothetie, 1./x, 1./y, 1./z);
	mat4_homothety(norm_homothetie, y/z, z/x, x/y);
	tree_transform(tree, homothetie, inv_homothetie, norm_homothetie);
	rescale_node(tree, x, y, z);
}

void tree_rotation (Tree tree, double x, double y, double z) {
	assert(NULL != tree);
	assert(tree_is_valid(tree));
	mat4 rotation, inv_rotation;
	mat4_rotation_x(rotation, x);
	mat4_rotation_x(inv_rotation, -x);
	tree_transform(tree, rotation, inv_rotation, rotation);
	mat4_rotation_y(rotation, y);
	mat4_rotation_y(inv_rotation, -y);
	tree_transform(tree, rotation, inv_rotation, rotation);
	mat4_rotation_z(rotation, z);
	mat4_rotation_z(inv_rotation, -z);
	tree_transform(tree, rotation, inv_rotation, rotation);
}

static int tree_contains_point (Tree tree, point3 *point) {
	assert(NULL != tree);
	assert(tree_is_valid(tree));
	assert(NULL != point);
	point3 p;
	mat4_product_point3(p, tree->inv_transformations, *point);
	if(tree->shape != NULL){
		return shape_contains_point(tree->shape, (const point3 *) &p);
	}
	switch (tree->op) {
		case Union: 
			return tree_contains_point(tree->left, &p) || tree_contains_point(tree->right, &p);
		case Intersection:
			return tree_contains_point(tree->left, &p) && tree_contains_point(tree->right, &p);
		case Difference:
			return tree_contains_point(tree->left, &p) && !tree_contains_point(tree->right, &p);
		case Identity:
			return tree_contains_point(tree->left, &p) || tree_contains_point(tree->right, &p);		
		default:
			fprintf(stderr, "Invalid Tree Operator descriptor '%u' (line %d file %s)", tree->op, __LINE__, __FILE__);
			exit(EXIT_FAILURE);
	}
}

static PointCloud * op_identity(mat4 transformations, mat4 norm_transformations, Tree left, Tree right, int density) {
	assert(NULL != left); 
	assert(tree_is_valid(left)); 
	assert(NULL != right); 
	assert(tree_is_valid(right)); 
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	color4 *colors = NULL;
	PointCloud *a = tree_to_point_cloud(left, density);
	PointCloud *b = tree_to_point_cloud(right, density);
	int size = a->size + b->size;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (colors = (color4 *) malloc(size * sizeof(color4)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	int i;
	for(i = 0; i < a->size; i++){
		mat4_product_point3(vrtx[i], transformations, a->vrtx[i]);
		mat4_product_vec3(norm[i], norm_transformations, a->norm[i]);
		color4_copy(colors[i], a->colors[i]);
	}
	for(i = 0; i < b->size; i++){
		mat4_product_point3(vrtx[i + a->size], transformations, b->vrtx[i]);
		mat4_product_vec3(norm[i + a->size], norm_transformations, b->norm[i]);
		color4_copy(colors[(i+a->size)], b->colors[i]);
	}
	point_cloud_free(&a);
	point_cloud_free(&b);
	return point_cloud_allocate(vrtx, norm, colors, size);
}

static PointCloud * op_union(mat4 transformations, mat4 norm_transformations, Tree left, Tree right, int density) {
	assert(NULL != left); 
	assert(tree_is_valid(left)); 
	assert(NULL != right); 
	assert(tree_is_valid(right)); 
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	color4 *colors = NULL;
	PointCloud *a = tree_to_point_cloud(left, density);
	PointCloud *b = tree_to_point_cloud(right, density);
	int size = a->size + b->size;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (colors = (color4 *) malloc(size * sizeof(color4)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	size = 0;
	int i;
	for(i = 0; i < a->size; i++){
		if(!tree_contains_point(right, a->vrtx + i)) {
			mat4_product_point3(vrtx[size], transformations, a->vrtx[i]);
			mat4_product_vec3(norm[size], norm_transformations, a->norm[i]);
			color4_copy(colors[size], a->colors[i]);
			size++;
		}
	}
	for(i = 0; i < b->size; i++){
		if(!tree_contains_point(left, b->vrtx + i)) {
			mat4_product_point3(vrtx[size], transformations, b->vrtx[i]);
			mat4_product_vec3(norm[size], norm_transformations, b->norm[i]);
			color4_copy(colors[size], b->colors[i]);
			size++;
		} 
	}
	point_cloud_free(&a);
	point_cloud_free(&b);
	return point_cloud_allocate(vrtx, norm, colors, size);
}

static PointCloud * op_intersection(mat4 transformations, mat4 norm_transformations, Tree left, Tree right, int density) {
	assert(NULL != left); 
	assert(tree_is_valid(left)); 
	assert(NULL != right); 
	assert(tree_is_valid(right)); 
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	color4 *colors = NULL;
	PointCloud *a = tree_to_point_cloud(left, density);
	PointCloud *b = tree_to_point_cloud(right, density);
	int size = a->size + b->size;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (colors = (color4 *) malloc(size * sizeof(color4)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	size = 0;
	int i;
	for(i = 0; i < a->size; i++){
		if(tree_contains_point(right, a->vrtx + i)) {
			mat4_product_point3(vrtx[size], transformations, a->vrtx[i]);
			mat4_product_vec3(norm[size], norm_transformations, a->norm[i]);
			color4_copy(colors[size], a->colors[i]);
			size++;
		}
	}
	for(i = 0; i < b->size; i++){
		if(tree_contains_point(left, b->vrtx + i)) {
			mat4_product_point3(vrtx[size], transformations, b->vrtx[i]);
			mat4_product_vec3(norm[size], norm_transformations, b->norm[i]);
			color4_copy(colors[size], b->colors[i]);
			size++;
		} 
	}
	point_cloud_free(&a);
	point_cloud_free(&b);
	return point_cloud_allocate(vrtx, norm, colors, size);
}

static PointCloud * op_difference(mat4 transformations, mat4 norm_transformations, Tree left, Tree right, int density) {
	assert(NULL != left); 
	assert(tree_is_valid(left)); 
	assert(NULL != right); 
	assert(tree_is_valid(right)); 
	point3 *vrtx = NULL;
	vec3 *norm = NULL;
	color4 *colors = NULL;
	PointCloud *a = tree_to_point_cloud(left, density);
	PointCloud *b = tree_to_point_cloud(right, density);
	int size = a->size + b->size;
	if (NULL == (vrtx = (point3 *) malloc(size * sizeof(point3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (norm = (vec3 *) malloc(size * sizeof(vec3)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	if (NULL == (colors = (color4 *) malloc(size * sizeof(color4)))) {
		fprintf(stderr, "memory allocation error (line %d file %s)", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	size = 0;
	int i;
	for(i = 0; i < a->size; i++){
		if(!tree_contains_point(right, a->vrtx + i)) {
			mat4_product_point3(vrtx[size], transformations, a->vrtx[i]);
			mat4_product_vec3(norm[size], norm_transformations, a->norm[i]);
			color4_copy(colors[size], a->colors[i]);
			size++;
		}
	}
	for(i = 0; i < b->size; i++){
		if(tree_contains_point(left, b->vrtx + i)) {
			mat4_product_point3(vrtx[size], transformations, b->vrtx[i]);
			vec3_set(
				norm[size],
				-(vec3_get_x(b->norm[i])),
				-(vec3_get_y(b->norm[i])),
				-(vec3_get_z(b->norm[i]))
			);
			mat4_product_vec3(norm[size], norm_transformations, norm[size]);
			color4_copy(colors[size], b->colors[i]);
			size++;
		} 
	}
	point_cloud_free(&a);
	point_cloud_free(&b);
	return point_cloud_allocate(vrtx, norm, colors, size);
}

PointCloud * tree_to_point_cloud(Tree tree, int density) {
	assert(NULL != tree); 
	assert(tree_is_valid(tree)); 
	if(tree->shape != NULL){
		return shape_to_point_cloud(tree->shape, density, tree->transformations, tree->norm_transformations);
	}
	switch (tree->op) {
		case Union:
			return op_union(tree->transformations, tree->norm_transformations, tree->left, tree->right, density);
		case Intersection:
			return op_intersection(tree->transformations, tree->norm_transformations, tree->left, tree->right, density);
		case Difference:
			return op_difference(tree->transformations, tree->norm_transformations, tree->left, tree->right, density);
		case Identity:
			return op_identity(tree->transformations, tree->norm_transformations, tree->left, tree->right, density);		
		default:
			fprintf(stderr, "Invalid Tree Operator descriptor '%u' (line %d file %s)", tree->op, __LINE__, __FILE__);
			exit(EXIT_FAILURE);
	}
}

void tree_free (Tree *tree) {
	assert(NULL != tree);
	assert(NULL != (*tree));
	assert(tree_is_valid(*tree));
	if((*tree)->shape != NULL){
		shape_free(&((*tree)->shape));
	} else {
		tree_free(&((*tree)->left));
		tree_free(&((*tree)->right));
	}	
	free((*tree));
	(*tree) = NULL;
}
