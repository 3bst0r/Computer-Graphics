/******************************************************************
*
* Lightsource.h
*
* Description: Class representing a point light with its coordinates 
* 			   and RGB color values
*
*******************************************************************/

#ifndef __LIGHTSOURCE_H__
	#define __LIGHTSOURCE_H__
	#define GLM_FORCE_RADIANS
	
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include "../glm/glm.hpp"
	#include "../glm/gtc/matrix_transform.hpp"
	#include "../glm/gtc/type_ptr.hpp"
	
	class Lightsource {
		public:
			Lightsource(float x_offset, float y_offset, float z_offset, float color_r, float color_g, float color_b);
			~Lightsource();
			glm::vec3 pos;
			glm::vec3 rgb;
			GLushort* index_buffer_data;
			glm::vec3 rgbToHsv(glm::vec3 rgbVal);
			glm::vec3 hsvToRgb(glm::vec3 hsvVal);
	};
	
#endif
