#ifndef COMPUTER_GRAPHICS_BILLBOARD_H
#define COMPUTER_GRAPHICS_BILLBOARD_H

#define GLM_FORCE_RADIANS
#include "Camera.h"
#include "Shape.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/string_cast.hpp"

class Billboard : public Shape {
    public:
        Billboard(glm::vec3 pos, float width, float height, Camera* c);
        ~Billboard();
        glm::mat4 getPosition();

    private:
        glm::vec3 pos;
        Camera* c;
};


#endif //COMPUTER_GRAPHICS_BILLBOARD_H
