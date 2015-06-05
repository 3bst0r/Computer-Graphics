//
// Created by johannes on 5/13/15.
//

#define GLM_FORCE_RADIANS

#include <iostream>
#include "Camera.h"
#include "Transformation.h"
#include "CameraMode.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/string_cast.hpp"


using namespace std;

Camera::Camera(glm::vec3 camera_pos) {
    mouseSpeed = 0.07;
    up = glm::vec3(0,1,0);
    eye = camera_pos;
    ctr = glm::vec3(0,0,0);
    focusOnCenter();
}

/* translate value along normalized u, v or w */
void Camera::translate(glm::vec3 axis, float value) {
    eye = glm::vec3(glm::translate(glm::mat4(1),value * axis) * glm::vec4(eye,1));
}

void Camera::forward(float value) {
    translate(w, value);
}

void Camera::back(float value) {
    translate(w, -value);
}

void Camera::left(float value) {
    translate(glm::cross(w, up), -value);
}

void Camera::right(float value) {
    translate(glm::cross(w, up), value);
}

void Camera::rotate(float degree, glm::vec3 axis) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(degree), axis);
    if(abs(glm::vec3(rotation * glm::vec4(w, 1))[1]) > 0.9){    //max angle 0.8 * PI/2
        return;
    }
    w = glm::vec3(rotation * glm::vec4(w, 1));
}

void Camera::rotateAroundCenter(float degree,glm::vec3 axis) {
    glm::vec3 eye_old = eye;
    eye = glm::vec3(glm::rotate(glm::mat4(1), glm::radians(degree), axis) * glm::vec4(eye,1));
    focusOnCenter();
    if(abs(w[1]) > 0.9){    //max angle 0.8 * PI/2
        eye = eye_old;
        focusOnCenter();
    }
}

void Camera::SetViewByMouse(float xOffset, float yOffset,CameraMode camera_mode) {
    switch(camera_mode) {
        case SEMI:
            rotateAroundCenter(-xOffset * mouseSpeed, up);
            rotateAroundCenter(-yOffset * mouseSpeed, glm::cross(w, up));
            break;
        case MANUAL:
            rotate(-xOffset * mouseSpeed, up);
            rotate(-yOffset * mouseSpeed, glm::cross(w, up));
            break;
        default:
            return;
    }
}

glm::vec3 Camera::getU(){
    return glm::cross(w, up);
}

void Camera::focusOnCenter() {
    w = glm::normalize(ctr - eye);
}

glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(eye,eye + w,up);
}

