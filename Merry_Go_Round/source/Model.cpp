/******************************************************************
*
* Model.cpp
*
* Description: Class to create a shape from a loaded model
*
*******************************************************************/


#include <iostream>
#include <cmath>
#include <time.h>

#include "Shape.h"
#include "Model.h"
#include "OBJParser.h"

using namespace std;

/******************************************************************
*
* Model::Model
*
* loads the model into a shape object
*
*******************************************************************/
Model::Model(obj_scene_data model, float x, float y, float z, float scale)
: Shape(model.vertex_count, model.face_count){
	srand(time(NULL));
	float color;
	//set random color
	for(int i = 0; i < 3 * model.vertex_count; i++){
		color = ((float)rand()) / RAND_MAX;
		color_buffer_data[i] = color;
	}

	center_x = x;
	center_y = y;
	center_z = z;
    for(int i = 0; i < model.vertex_count; i++){
        vertex_buffer_data[i * 3] = (GLfloat)(*model.vertex_list[i]).e[0] * scale + x;
		vertex_buffer_data[i * 3 + 1] = (GLfloat)(*model.vertex_list[i]).e[1] * scale + y;
		vertex_buffer_data[i * 3 + 2] = (GLfloat)(*model.vertex_list[i]).e[2] * scale + z;
    }
    
    for(int i = 0; i < model.face_count; i++){
		index_buffer_data[i*3] = (GLushort)(*model.face_list[i]).vertex_index[0];
		index_buffer_data[i*3+1] = (GLushort)(*model.face_list[i]).vertex_index[1];
		index_buffer_data[i*3+2] = (GLushort)(*model.face_list[i]).vertex_index[2];
    }
}

/******************************************************************
*
* Model::~Model
*
* default destructor
*
*******************************************************************/
Model::~Model(){

}
