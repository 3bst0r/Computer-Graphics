/******************************************************************
*
* Model.h
*
* Description: Class to create a shape from a loaded model
*
*******************************************************************/

#ifndef __MODEL_H__
	#define __MODEL_H__
	
	#include "OBJParser.h"
	
	class Model : public Shape {
		public:
			Model(obj_scene_data model, float x, float y, float z, float scale);
			~Model();
	};
	
#endif
