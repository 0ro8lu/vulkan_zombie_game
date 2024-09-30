#ifndef _ENTITY_H_
#define _ENTITY_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct Entity {
  glm::vec2 position;
  float health;
  float speed;
};

#endif
