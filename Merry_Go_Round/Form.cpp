#include <iostream>

#include "Form.h"

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

void Form::print_indices(){
	for(int i = 0; i < triangle_number; i++){
		cout << "[1] = " << index_buffer_data[3 * i] << " "
			 << "[2] = " << index_buffer_data[3 * i + 1] << " " 
			 << "[3] = " << index_buffer_data[3 * i + 2] << endl;
	}
}
