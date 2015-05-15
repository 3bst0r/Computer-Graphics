//
// Created by johannes on 5/13/15.
//

#define GLM_FORCE_RADIANS

#include <iostream>
#include "Camera.h"
#include "Transformation.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

using namespace std;

Camera::Camera(glm::vec3 camera_pos) {
    eye = camera_pos;
    u = glm::vec3(1,0,0);
    v = glm::vec3(0,1,0);
    w = glm::vec3(0,0,1);
    ctr = glm::vec3(0,0,0);
    w = glm::normalize(ctr - eye);
    viewMatrix = glm::lookAt(eye,    /* Eye vector */
                            ctr,     /* Viewing center */
                            glm::vec3(0,1,0));    /* up vector */
    /* define the local coordinate system */
    glm::mat4 inv = glm::inverse(viewMatrix);
    u = glm::normalize(glm::vec3(inv * glm::vec4(u,0)));
    v = glm::normalize(glm::vec3(inv * glm::vec4(v,0)));
    w = glm::normalize(glm::vec3(inv * glm::vec4(w,0)));
}

/* translate value along normalized u, v or w */
void Camera::translate(glm::vec3 axis, float value) {
    glm::mat4 eye_transformation = glm::translate(glm::mat4(1),value * axis);
    viewMatrix = glm::inverse(eye_transformation) * viewMatrix;
    eye = glm::vec3(eye_transformation * glm::vec4(eye,1));
}

void Camera::forward(float value) {
    translate(w,value);
}

void Camera::back(float value) {
    translate(w,-value);
}

void Camera::left(float value) {
    translate(u,-value);
}

void Camera::right(float value) {
    translate(u,value);
}

/* rotate around u, v or w */
void Camera::rotate(float degree, glm::vec3 axis) {
    glm::mat4 eye_transformation = glm::rotate(glm::mat4(1),glm::radians(degree),axis);
    u = glm::normalize(glm::vec3(eye_transformation * glm::vec4(u,0)));
    v = glm::normalize(glm::vec3(eye_transformation * glm::vec4(v,0)));
    w = glm::normalize(glm::vec3(eye_transformation * glm::vec4(w,0)));
    viewMatrix = glm::inverse(eye_transformation) * viewMatrix;
}

void Camera::rotateU(float degree) {
    rotate(degree,u);
}

void Camera::rotateV(float degree) {
    rotate(degree,v);
}

void Camera::rotateW(float degree) {
    rotate(degree,w);
}

void Camera::rotateAroundCenter(float degrees) {

}


