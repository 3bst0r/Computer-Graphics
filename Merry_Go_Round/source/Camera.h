//
// Created by johannes on 5/13/15.
//

#ifndef COMPUTER_GRAPHICS_CAMERA_H
#define COMPUTER_GRAPHICS_CAMERA_H
#define GLM_FORCE_RADIANS
#include "../glm/glm.hpp"
#include "CameraMode.h"

class Camera {
public:
    Camera(glm::vec3 camera_pos);
    glm::vec3 eye, ctr, w;

    /* the view matrix to be loaded into the shader */
    void translate(glm::vec3 axis, float value);
    void forward(float value);
    void right(float value);
    void left(float value);
    void back(float value);
    void rotate(float degrees, glm::vec3 axis);
    void rotateAroundCenter(float degrees,  glm::vec3 axis);
    void focusOnCenter();
    void SetViewByMouse(float xOffset, float yOffset,CameraMode camera_mode);
    glm::vec3 getU();
    glm::mat4 viewMatrix();
private:
    float mouseSpeed;
    glm::vec3 up;
};


#endif //COMPUTER_GRAPHICS_CAMERA_H
