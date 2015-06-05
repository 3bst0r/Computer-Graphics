/******************************************************************
*
* Shape.cpp
*
* Description: Class to create basic shapes
*
*******************************************************************/

#define GLM_FORCE_RADIANS

#include <iostream>
#include <string.h>
#include "Shape.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/string_cast.hpp"

using namespace std;

/******************************************************************
*
* Shape::Shape
*
* creates a shape object with given specification
*
*******************************************************************/
Shape::Shape(int vertex_number, int triangle_number){
	this->vertex_number = vertex_number;
	this->triangle_number = triangle_number;
	vertex_buffer_data = (GLfloat*)malloc(3 * vertex_number * sizeof(GLfloat));
	color_buffer_data = (GLfloat*)malloc(3 * vertex_number * sizeof(GLfloat));
	normal_buffer_data = (GLfloat*)calloc(3 * vertex_number, sizeof(GLfloat));
	index_buffer_data = (GLushort*)malloc(3 * triangle_number * sizeof(GLushort));
	kS = 0.3;
	kD = 0.2;
	kA = 0.2;
}

/******************************************************************
*
* Shape::~Shape
*
* free the allocated memory
*
*******************************************************************/
Shape::~Shape(){
	if(vertex_buffer_data != NULL)
		free(vertex_buffer_data);
	if(color_buffer_data != NULL)
		free(color_buffer_data);
	if(index_buffer_data != NULL)
		free(index_buffer_data);
	if(normal_buffer_data != NULL)
		free(normal_buffer_data);
}

void Shape::compute_normals(){
	for(int i = 0; i < 3 * vertex_number; i++){
		normal_buffer_data[i] = 0.;
	}
	
	for(int i = 0; i < triangle_number; i++){
		glm::vec3 v1 = glm::vec3(vertex_buffer_data[3 * index_buffer_data[3 * i]], vertex_buffer_data[3 * index_buffer_data[3 * i] + 1], vertex_buffer_data[3 * index_buffer_data[3 * i] + 2]);
		glm::vec3 v2 = glm::vec3(vertex_buffer_data[3 * index_buffer_data[3 * i + 1]], vertex_buffer_data[3 * index_buffer_data[3 * i + 1] + 1], vertex_buffer_data[3 * index_buffer_data[3 * i + 1] + 2]);
		glm::vec3 v3 = glm::vec3(vertex_buffer_data[3 * index_buffer_data[3 * i + 2]], vertex_buffer_data[3 * index_buffer_data[3 * i + 2] + 1], vertex_buffer_data[3 * index_buffer_data[3 * i + 2] + 2]);
		glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
		
		normal_buffer_data[3 * index_buffer_data[3 * i]] += normal[0];
		normal_buffer_data[3 * index_buffer_data[3 * i] + 1] += normal[1];
		normal_buffer_data[3 * index_buffer_data[3 * i] + 2] += normal[2];
		
		normal_buffer_data[3 * index_buffer_data[3 * i + 1]] += normal[0];
		normal_buffer_data[3 * index_buffer_data[3 * i + 1] + 1] += normal[1];
		normal_buffer_data[3 * index_buffer_data[3 * i + 1] + 2] += normal[2];
		
		normal_buffer_data[3 * index_buffer_data[3 * i + 2]] += normal[0];
		normal_buffer_data[3 * index_buffer_data[3 * i + 2] + 1] += normal[1];
		normal_buffer_data[3 * index_buffer_data[3 * i + 2] + 2] += normal[2];
	}
	
	for(int i = 0; i < vertex_number; i++){
		glm::vec3 normalized = glm::normalize(-glm::vec3(normal_buffer_data[3 * i], normal_buffer_data[3 * i + 1], normal_buffer_data[3 * i + 2]));
		normal_buffer_data[3 * i] = normalized[0];
		normal_buffer_data[3 * i + 1] = normalized[1];
		normal_buffer_data[3 * i + 2] = normalized[2];
	}
}

void Shape::print_normals(){
	for(int i = 0; i < vertex_number; i++){
		cout << "normal ";
		for(int j = 0; j < 3; j++)
			cout << normal_buffer_data[3* i + j] << " ";
		cout << endl;
	}
}

/******************************************************************
*
* Shape::print_vertices
*
* prints all vertices of the shape
*
*******************************************************************/
void Shape::print_vertices(){
	for(int i = 0; i < vertex_number; i++){
		cout << i << ": " << "[x] = " << vertex_buffer_data[3 * i] << " "
			 << "[y] = " << vertex_buffer_data[3 * i + 1] << " " 
			 << "[z] = " << vertex_buffer_data[3 * i + 2] << endl;
	}
}

/******************************************************************
*
* Shape::print_indices
*
* prints all indices of the shape
*
*******************************************************************/
void Shape::print_indices() {
	for (int i = 0; i < triangle_number; i++) {
		cout << "[1] = " << index_buffer_data[3 * i] << " "
			 << "[2] = " << index_buffer_data[3 * i + 1] << " "
			 << "[3] = " << index_buffer_data[3 * i + 2] << endl;
	}
}

/******************************************************************
*
* Shape::add_shape
*
* adds a given shape to this
* deletes the given shape at the end
*
*******************************************************************/
void Shape::add_shape(Shape* shape) {
	/* reallocate all the new memory */
	GLfloat* vertex_buffer_data_t = (GLfloat*) realloc(vertex_buffer_data, 3 * vertex_number * sizeof(GLfloat) + 3 * shape->vertex_number * sizeof(GLfloat));
	GLfloat* color_buffer_data_t = (GLfloat*) realloc(color_buffer_data, 3 * vertex_number * sizeof(GLfloat) + 3 * shape->vertex_number * sizeof(GLfloat));
	GLfloat* normal_buffer_data_t = (GLfloat*) realloc(normal_buffer_data, 3 * vertex_number * sizeof(GLfloat) + 3 * shape->vertex_number * sizeof(GLfloat));
	GLushort* index_buffer_data_t =  (GLushort*) realloc(index_buffer_data, 3 * triangle_number * sizeof(GLushort) + 3 * shape->triangle_number * sizeof(GLushort));
	
	/* check if realloc was ok */
	if (index_buffer_data_t != NULL && color_buffer_data_t != NULL && index_buffer_data_t != NULL && normal_buffer_data_t != NULL) {
		vertex_buffer_data = vertex_buffer_data_t;
		color_buffer_data = color_buffer_data_t;
		index_buffer_data = index_buffer_data_t;
		normal_buffer_data = normal_buffer_data_t;
	} else {
		cerr << "error reallocating memory";
		exit(-1);
	}
	
	/* append the new form's contents */
	memcpy(vertex_buffer_data + 3 * vertex_number,shape->vertex_buffer_data,3 * shape->vertex_number * sizeof(GLfloat));
	memcpy(color_buffer_data + 3 * vertex_number,shape->color_buffer_data,3 * shape->vertex_number * sizeof(GLfloat));
	memcpy(normal_buffer_data + 3 * vertex_number,shape->normal_buffer_data,3 * shape->vertex_number * sizeof(GLfloat));
	memcpy(index_buffer_data + 3 * triangle_number,shape->index_buffer_data,3 * shape->triangle_number * sizeof(GLushort));
	
	/* add an offset of triangle_number to all the new indices */
	for(int i = 3 * triangle_number;i < 3 * (triangle_number + shape->triangle_number); i++) {
		index_buffer_data[i] += vertex_number;
	}
	/* calculate the new vertex and triangle number */
	vertex_number += shape->vertex_number;
	triangle_number += shape->triangle_number;
	
	/* delete the given shape */
	delete shape;
}

