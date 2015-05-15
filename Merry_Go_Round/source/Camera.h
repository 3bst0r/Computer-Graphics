//
// Created by johannes on 5/13/15.
//

#ifndef COMPUTER_GRAPHICS_CAMERA_H
#define COMPUTER_GRAPHICS_CAMERA_H

#include "../glm/glm.hpp"

class Camera {
public:
    Camera(glm::vec3 camera_pos);
    glm::vec3 eye, ctr, u, v, w;

    /* the view matrix to be loaded into the shader */
    glm::mat4 viewMatrix;
    void translate(glm::vec3 axis, float value);
    void forward(float value);
    void right(float value);
    void left(float value);
    void back(float value);
    void rotate(float degrees, glm::vec3 axis);
    void rotateAroundCenter(float degrees,  glm::vec3 axis);
    void focusOnCenter();
};


#endif //COMPUTER_GRAPHICS_CAMERA_H
