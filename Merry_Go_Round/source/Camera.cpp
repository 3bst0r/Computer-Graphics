//
// Created by johannes on 5/13/15.
//

#define GLM_FORCE_RADIANS

#include <iostream>
#include "Camera.h"
#include "Transformation.h"
#define GLM_FORCE_RADIANS
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/string_cast.hpp"


using namespace std;

Camera::Camera(glm::vec3 camera_pos) {
    mouseSpeed = 0.01;
    up = glm::vec3(0,1,0);
    eye = camera_pos;
    _u = glm::vec3(1,0,0);
    _v = glm::vec3(0,1,0);
    _w = glm::vec3(0,0,-1);
    u = _u;
    v = _v;
    w = _w;
    ctr = glm::vec3(0,0,0);
    focusOnCenter();
}

/* translate value along normalized u, v or w */
void Camera::translate(glm::vec3 axis, float value) {
    glm::mat4 eye_transformation = glm::translate(glm::mat4(1),value * axis);
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

//void Camera::move(Camera intit, float angle

/* rotate around u, v or w */
void Camera::rotate(float degree, glm::vec3 axis) {
    glm::mat4 rotate = glm::rotate(glm::mat4(1),glm::radians(degree),axis);
    w = glm::vec3(rotate * glm::vec4(w, 0));
    glm::mat4 inv = glm::inverse(viewMatrix());

    u = glm::vec3(inv * glm::vec4(_u, 0));
    v = glm::vec3(inv * glm::vec4(_v, 0));
}

void Camera::rotateAroundCenter(float degree,glm::vec3 axis) {
    glm::mat4 eye_transformation = glm::translate(glm::mat4(1),eye-ctr);
    eye_transformation = glm::rotate(eye_transformation,glm::radians(degree),axis);
    eye_transformation = glm::translate(eye_transformation,ctr-eye);

    eye = glm::vec3(eye_transformation * glm::vec4(eye,1));
    u = glm::normalize(glm::vec3(eye_transformation * glm::vec4(u,0)));
    v = glm::normalize(glm::vec3(eye_transformation * glm::vec4(v,0)));
    w = glm::normalize(glm::vec3(eye_transformation * glm::vec4(w,0)));
}

void Camera::SetViewByMouse(float xOffset, float yOffset) {

}
void Camera::focusOnCenter() {
    glm::mat4 _viewMatrix = glm::lookAt(eye,    /* Eye vector */
                             ctr,     /* Viewing center */
                             up);    /* up vector */
    /* define the local coordinate system */
    glm::mat4 inv = glm::inverse(_viewMatrix);
    u = glm::normalize(glm::vec3(inv * glm::vec4(u,0)));
    v = glm::normalize(glm::vec3(inv * glm::vec4(v,0)));
    w = glm::normalize(glm::vec3(inv * glm::vec4(w,0)));
}

glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(eye,eye + w,up);
}

