/******************************************************************
*
* Block.h
*
* Description: Class to create a block-like shape with variable side lengths
*
*******************************************************************/

#ifndef __BLOCK_H__
	#define __BLOCK_H__
	
	class Block : public Shape {
		public:
			Block(float x_val, float y_val, float z_val, float height, float width, float thickness);
			~Block();
	};
	
#endif
