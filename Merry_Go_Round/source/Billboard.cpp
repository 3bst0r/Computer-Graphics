
#define GLM_FORCE_RADIANS

#include <iostream>
#include <cmath>

#include "Billboard.h"
#include "Camera.h"
#include "Shape.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/string_cast.hpp"

using namespace std;

Billboard::Billboard(glm::vec3 pos, float width, float height, Camera* c)
:Shape(4, 2) {
    this->pos = pos;
    this->c = c;

    for (int i = 0; i < 4 * 3; i++) {
        color_buffer_data[i] = 1.;
    }

    //bottom left
    vertex_buffer_data[0] = width / 2;
    vertex_buffer_data[1] = 0.;
    vertex_buffer_data[2] = 0.;
    uv_buffer_data[0] = 0.;
    uv_buffer_data[1] = 0.;

    //bottom right
    vertex_buffer_data[3] = - width / 2;
    vertex_buffer_data[4] = 0.;
    vertex_buffer_data[5] = 0.;
    uv_buffer_data[2] = 1.;
    uv_buffer_data[3] = 0.;

    //top left
    vertex_buffer_data[6] = width / 2;
    vertex_buffer_data[7] = height;
    vertex_buffer_data[8] = 0.;
    uv_buffer_data[4] = 0.;
    uv_buffer_data[5] = 1.;

    //top right
    vertex_buffer_data[9] = - width / 2;
    vertex_buffer_data[10] = height;
    vertex_buffer_data[11] = 0.;
    uv_buffer_data[6] = 1.;
    uv_buffer_data[7] = 1.;

    index_buffer_data[0] = 0;
    index_buffer_data[1] = 2;
    index_buffer_data[2] = 1;

    index_buffer_data[3] = 1;
    index_buffer_data[4] = 2;
    index_buffer_data[5] = 3;

    compute_normals();
}

glm::mat4 Billboard::getPosition(){
    glm::mat4 ret;

    glm::vec3 n = glm::normalize(c->eye - pos);
    glm::vec3 r = glm::normalize(glm::cross(glm::vec3(0,1,0), n));
    glm::vec3 up = glm::cross(n, r);

    ret[2] = glm::vec4(n, 0);
    ret[0] = glm::vec4(r, 0);
    ret[1] = glm::vec4(up, 0);
    ret[3] = glm::vec4(pos, 1);

    return ret;
}

Billboard::~Billboard() {

}