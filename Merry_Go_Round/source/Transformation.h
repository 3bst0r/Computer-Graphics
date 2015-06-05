/******************************************************************
*
* Transformation.h
*
* Description: Class to build a transformation matrix + function to set
* 			   set the perspective matrix
*
*******************************************************************/

#ifndef __TRANSFORMATION_H__
	#define __TRANSFORMATION_H__

	class Transformation{
		public:
			float* matrix;
			Transformation();
			~Transformation();
			void rotateX(float angle);
			void rotateY(float angle);
			void rotateZ(float angle);
			void translate(float x, float y, float z);
			void set_transformation(const float* m);
			void multiply(const float* matrix2);
	};

	void SetPerspectiveMatrix(float fov, float aspect, float nearPlane, float farPlane, float* result);

#endif
