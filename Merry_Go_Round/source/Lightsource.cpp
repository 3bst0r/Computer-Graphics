#include "Lightsource.h"

using namespace std;

/******************************************************************
*
* Lightsource.h
*
* Description: Class representing a point light with its coordinates 
* 			   and RGB color values
*
*******************************************************************/

Lightsource::Lightsource(float x_offset, float y_offset, float z_offset, float color_r, float color_g, float color_b){
	
			pos = glm::vec3(x_offset, y_offset, z_offset);
			rgb = glm::vec3(color_r, color_g, color_b);
			index_buffer_data = (GLushort*)malloc(1 * sizeof(GLushort));
			index_buffer_data[0] = 0;
}

/******************************************************************
*
* Lightsource::~Lightsource
*
* free the allocated memory
*
*******************************************************************/
Lightsource::~Lightsource(){
	if(index_buffer_data != NULL)
		free(index_buffer_data);
}

/******************************************************************
*
* rgbToHsv
*
* converts rgb values to hsv
*
*******************************************************************/

glm::vec3 Lightsource::rgbToHsv(glm::vec3 rgbVal){
	
	float M = glm::max(glm::max(rgbVal[0], rgbVal[1]), rgbVal[2]);
	float N = glm::min(glm::min(rgbVal[0], rgbVal[1]), rgbVal[2]);
	float h;
	float s;
	float v;
	
	/*compute H*/
	if(M == N){
		h = 0;
	}else if(M == rgbVal.x){
		h = 60 * (0 + (rgbVal[1] - rgbVal[2])/(M - N));
	}else if(M == rgbVal.y){
		h = 60 * (2 + (rgbVal[2] - rgbVal[0])/(M - N));
	}else{
		h = 60 * (4 + (rgbVal[0] - rgbVal[2])/(M - N));
	}
	if(h <0){
		h += 360;
	}
	
	/*compute S*/
	if(M == 0){
		s = 0;
	}else{
		s = 100 * (M - N)/M;
	}
	
	/*compute V*/
	v = 100 * M;
	
	return glm::vec3(h,s,v);
}

/******************************************************************
*
* hsvToRgb
*
* converts hsv values to rgb
*
*******************************************************************/

glm::vec3 Lightsource::hsvToRgb(glm::vec3 hsvVal){
	
	float C = hsvVal[2] * hsvVal[1];
	float H = hsvVal[0]/60;
	float X = C * (1 -	glm::abs(((int)round(H) % 2) - 1)); 
	float m = hsvVal[2] - C;
	
	
	if(0 <= H && H < 1){
		return glm::vec3(C + m,X + m,0 + m);
	}else if(1 <= H  && H < 2){
		return glm::vec3(X + m,C + m,0 + m);
	}else if(2 <= H && H < 3){
		return glm::vec3(0 + m,C + m,X + m);
	}else if(3 <= H && H < 4){
		return glm::vec3(0 + m,X + m,C + m);
	}else if(4 <= H && H < 5){
		return glm::vec3(X + m,0 + m,C + m);
	}else if(5 <= H && H < 6){
		return glm::vec3(C + m,0 + m,X + m);
	}else{
		return glm::vec3(0 + m,0 + m,0 + m);
	}
}
