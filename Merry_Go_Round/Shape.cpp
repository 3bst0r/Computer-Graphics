#include <iostream>
#include <string.h>
#include "Shape.h"

using namespace std;

Form::Form(int vertex_number, int triangle_number){
	cout << "Creating form with: vertex_number " << vertex_number << " triangle_number: " << triangle_number << endl;
	this->vertex_number = vertex_number;
	this->triangle_number = triangle_number;
	vertex_buffer_data = (GLfloat*)malloc(3 * vertex_number * sizeof(GLfloat));
	color_buffer_data = (GLfloat*)malloc(3 * vertex_number * sizeof(GLfloat));
	index_buffer_data = (GLushort*)malloc(3 * triangle_number * sizeof(GLushort));
}

Form::~Form(){
	if(vertex_buffer_data != NULL)
		free(vertex_buffer_data);
	if(color_buffer_data != NULL)
		free(color_buffer_data);
	if(index_buffer_data != NULL)
		free(index_buffer_data);
}
void Form::print_vertices(){
	for(int i = 0; i < vertex_number; i++){
		cout << i << ": " << "[x] = " << vertex_buffer_data[3 * i] << " "
			 << "[y] = " << vertex_buffer_data[3 * i + 1] << " " 
			 << "[z] = " << vertex_buffer_data[3 * i + 2] << endl;
	}
}

void Form::print_indices() {
	for (int i = 0; i < triangle_number; i++) {
		cout << "[1] = " << index_buffer_data[3 * i] << " "
														<< "[2] = " << index_buffer_data[3 * i + 1] << " "
														<< "[3] = " << index_buffer_data[3 * i + 2] << endl;
	}
}

/*
 * adds a new form to this form
 * the approach it uses is "absorb all the data from the new object and then kill it"
 */
void Form::add_form(Form form) {
	// reallocate all the new memory
	GLfloat* vertex_buffer_data_t = (GLfloat*) realloc(vertex_buffer_data, sizeof(vertex_buffer_data) + 3 * form.vertex_number * sizeof(GLfloat));
	GLfloat* color_buffer_data_t = (GLfloat*) realloc(color_buffer_data, sizeof(color_buffer_data) + 3 * form.vertex_number * sizeof(GLfloat));
	GLushort* index_buffer_data_t =  (GLushort*) realloc(index_buffer_data, sizeof(index_buffer_data) + 3 * form.triangle_number * sizeof(GLushort));

	if (index_buffer_data_t != NULL && color_buffer_data_t != NULL && index_buffer_data_t != NULL) {
		vertex_buffer_data = vertex_buffer_data_t;
		color_buffer_data = color_buffer_data_t;
		index_buffer_data = index_buffer_data_t;
	} else {
		cerr << "error reallocating memory";
		exit(-1);
	}

	// append the new form's contents
	memcpy(vertex_buffer_data+3*vertex_number,form.vertex_buffer_data,sizeof(form.vertex_buffer_data));
	memcpy(color_buffer_data+3*vertex_number,form.color_buffer_data,sizeof(form.color_buffer_data));
	memcpy(index_buffer_data+3*triangle_number,form.index_buffer_data,sizeof(form.index_buffer_data));

	// add an offset of 3*triangle_number to all the new indices
	for(int i = 3*triangle_number;i < 3*(triangle_number + form.triangle_number); i++) {
		index_buffer_data[i] += 3*triangle_number;
	}

	vertex_number += form.vertex_number;
	triangle_number += form.triangle_number;
	form.~Form();
}
