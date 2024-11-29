#ifndef _BULLET_H_
#define _BULLET_H_

#include "glm/glm.hpp"

struct Bullet {
    glm::vec2 position;
    glm::vec2 direction;
    float speed;
    // technically this should be called velocity but there's no time nor will to change it from Enity class too.
};

#endif
