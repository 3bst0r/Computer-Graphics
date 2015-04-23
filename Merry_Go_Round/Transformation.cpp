/******************************************************************
*
* Transformation.cpp
*
* Description: Class to build a transformation matrix + function to set
* 			   set the perspective matrix
*
*******************************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "Transformation.h"

using namespace std;

/******************************************************************
*
* Transformation::Transformation
*
* creates a new Transformation object and initializes it with the
* identity matrix
*
*******************************************************************/

Transformation::Transformation(){
	matrix = (float*)calloc(16, sizeof(float));
	
	if(matrix == NULL){
		cerr << "calloc returned NULL" << endl;
		exit(EXIT_FAILURE);
	}
	
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
}

/******************************************************************
*
* Transformation::~Transformation
*
* free the allocated memory
*
*******************************************************************/

Transformation::~Transformation(){
	if(matrix != NULL)
		free(matrix);
}

/******************************************************************
*
* Transformation::rotateX
*
* rotates the Transformation around the X-Axis with the given angle
*
*******************************************************************/

void Transformation::rotateX(float angle){
	angle = M_PI/180 * angle;
	float temp[16] = 
    {  
        1.0,           0.0,          0.0, 0.0,
		0.0,   cosf(angle), -sinf(angle), 0.0,
		0.0,   sinf(angle),  cosf(angle), 0.0,
		0.0,           0.0,          0.0, 1.0  
    };
    
    multiply(temp);
}

/******************************************************************
*
* Transformation::rotateY
*
* rotates the Transformation around the Y-Axis with the given angle
*
*******************************************************************/

void Transformation::rotateY(float angle){
	angle = M_PI/180 * angle;
	float temp[16] = 
    {
        cosf(angle),  0.0,   sinf(angle),  0.0,
	            0.0,  1.0,           0.0,  0.0,
       -sinf(angle),  0.0,   cosf(angle),  0.0,
	            0.0,  0.0,           0.0,  1.0   
    };
    
    multiply(temp);
}

/******************************************************************
*
* Transformation::rotateZ
*
* rotates the Transformation around the Z-Axis with the given angle
*
*******************************************************************/

void Transformation::rotateZ(float angle){
	angle = M_PI/180 * angle;
	float temp[16] = 
    {	
         cosf(angle),  -sinf(angle),  0.0,  0.0,
         sinf(angle),   cosf(angle),  0.0,  0.0,          
                 0.0,           0.0,  1.0,  0.0,
                 0.0,           0.0,  0.0,  1.0   
    };
    
    multiply(temp);
}

/******************************************************************
*
* Transformation::translate
*
* translate the Transformation by the given values
*
*******************************************************************/

void Transformation::translate(float x, float y, float z){
	float temp[16] = 
	{
		1.0,  0.0,  0.0,    x,
		0.0,  1.0,  0.0,    y,
		0.0,  0.0,  1.0,    z,
		0.0,  0.0,  0.0,  1.0
	};
	
	multiply(temp);
}


/******************************************************************
*
* Transformation::multiply
* 
* multipy the Transformation with a given matrix
*
*******************************************************************/

void Transformation::multiply(float* m2)
{
    float temp[16];

    temp[0] = matrix[0]*m2[0] + matrix[1]*m2[4] + matrix[2]*m2[8] + matrix[3]*m2[12];
    temp[1] = matrix[0]*m2[1] + matrix[1]*m2[5] + matrix[2]*m2[9] + matrix[3]*m2[13];
    temp[2] = matrix[0]*m2[2] + matrix[1]*m2[6] + matrix[2]*m2[10] + matrix[3]*m2[14];
    temp[3] = matrix[0]*m2[3] + matrix[1]*m2[7] + matrix[2]*m2[11] + matrix[3]*m2[15];

    temp[4] = matrix[4]*m2[0] + matrix[5]*m2[4] + matrix[6]*m2[8] + matrix[7]*m2[12];
    temp[5] = matrix[4]*m2[1] + matrix[5]*m2[5] + matrix[6]*m2[9] + matrix[7]*m2[13];
    temp[6] = matrix[4]*m2[2] + matrix[5]*m2[6] + matrix[6]*m2[10] + matrix[7]*m2[14];
    temp[7] = matrix[4]*m2[3] + matrix[5]*m2[7] + matrix[6]*m2[11] + matrix[7]*m2[15];

    temp[8] = matrix[8]*m2[0] + matrix[9]*m2[4] + matrix[10]*m2[8] + matrix[11]*m2[12];
    temp[9] = matrix[8]*m2[1] + matrix[9]*m2[5] + matrix[10]*m2[9] + matrix[11]*m2[13];
    temp[10] = matrix[8]*m2[2] + matrix[9]*m2[6] + matrix[10]*m2[10] + matrix[11]*m2[14];
    temp[11] = matrix[8]*m2[3] + matrix[9]*m2[7] + matrix[10]*m2[11] + matrix[11]*m2[15];

    temp[12] = matrix[12]*m2[0] + matrix[13]*m2[4] + matrix[14]*m2[8] + matrix[15]*m2[12];
    temp[13] = matrix[12]*m2[1] + matrix[13]*m2[5] + matrix[14]*m2[9] + matrix[15]*m2[13];
    temp[14] = matrix[12]*m2[2] + matrix[13]*m2[6] + matrix[14]*m2[10] + matrix[15]*m2[14];
    temp[15] = matrix[12]*m2[3] + matrix[13]*m2[7] + matrix[14]*m2[11] + matrix[15]*m2[15];

    memcpy(matrix, temp, 16*sizeof(float));
}

void Transformation::set_transformation(float* m){
	memcpy(matrix, m, 16 * sizeof(float));
}


/******************************************************************
*
* SetPerspectiveMatrix
* 
* sets the perspective matrix with the given values
*
*******************************************************************/

void SetPerspectiveMatrix(float fov, float aspect, float nearPlane, 
                          float farPlane, float* result) 
{   
    float f = 1.0 / tan(fov * 3.1415926/360.0);
    float c1 = -(farPlane + nearPlane) / (farPlane - nearPlane);
    float c2 = -(2.0 * farPlane * nearPlane) / (farPlane - nearPlane);

    float temp[16] = 
    {
        f/aspect,  0.0,   0.0,  0.0,
	     0.0,    f,   0.0,  0.0,
	     0.0,  0.0,    c1,   c2,
         0.0,  0.0,  -1.0,  0.0
    };

    memcpy(result, temp, 16*sizeof(float));
}


