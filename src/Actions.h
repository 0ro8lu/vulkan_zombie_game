#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include <variant>
#include <glm/glm.hpp>

struct Move {short x; short y;};
struct Shoot{};
struct Aim{glm::vec2 direction;};
struct LoadLevel{};

using Actions = std::variant<Move, Shoot, Aim, LoadLevel>;

#endif

