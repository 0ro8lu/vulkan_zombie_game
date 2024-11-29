#include "ZombieGame.h"
#include <fstream>
#include <stdlib.h>
#include <time.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

ZombieGame::ZombieGame()
{
    // levels.push_back("../../levels/level1");
    // levels.push_back("../../levels/level2");
    
 levels.push_back("../levels/level1");
 levels.push_back("../levels/level2");
  assert(levels.size() > 0);

  levelData.reserve(100);

  levelHeight = 0;
  levelWidth = 0;
  
  gameState = GameState::UNINITIALIZED;

  levelIndex = 0;
  srand (time(NULL));
}

ZombieGame::~ZombieGame() {
  resetParams();
}

void ZombieGame::update(const std::vector<Actions>& actions) {  
  if(gameState == GameState::GAMOVER) {
    for(const Actions& action : actions) {
      if(auto shoot = std::get_if<LoadLevel>(&action)) {
        // unload level and load new one
        resetParams();
        gameState = GameState::UNINITIALIZED;
      }
    }
  }

  if(gameState == GameState::VICTORY) {
    for(const Actions& action : actions) {
      if(auto shoot = std::get_if<LoadLevel>(&action)) {
        resetParams();
        levelIndex++;
        gameState = GameState::UNINITIALIZED;
      }
    }
  }

  if(gameState == GameState::RUNNING) {
    runGameLogic(actions);
  }  
}

void ZombieGame::runGameLogic(const std::vector<Actions>& actions) {
  
  // update bullet position and check for collision
  for(int i = 0; i < bullets.size(); i++) {
    bullets[i]->position = bullets[i]->position + bullets[i]->direction * bullets[i]->speed;

    // check collision with zombies
    for(int j = 0; j < zombies.size(); j++) {
      if(isBulletCollidingWithEntity(bullets[i]->position, zombies[j])) {
        // delete bullet
        delete bullets[i];
        bullets[i] = bullets.back();
        bullets.pop_back();
      }
    }
  }

  for(int i = 0; i < bullets.size(); i++) {
    // check collision with humans
    for(int j = 0; j < humans.size(); j++) {
      if(humans[j] == player) {
        continue;
      }
      if(isBulletCollidingWithEntity(bullets[i]->position, humans[j])) {
        // delete bullet
        delete bullets[i];
        bullets[i] = bullets.back();
        bullets.pop_back();
      }
    }
  }

  for(int i = 0; i < bullets.size(); i++) {
    // check collision with zombies
    if (isBulletCollidingWithWorld(bullets[i]->position)) {
      // delete bullet
      delete bullets[i];
      bullets[i] = bullets.back();
      bullets.pop_back();
    }
  }

  // process user movement
  for(const Actions& action : actions) {
    
    if(auto shoot = std::get_if<Shoot>(&action)) {
      if(bullets.size() < MAX_NUM_BULLETS) {
        auto direction = player->direction;
        auto position = player->position + glm::vec2(TILE_SCALE / 2, TILE_SCALE / 2);
        bullets.emplace_back(new Bullet{position, direction, 10});
      }
    }

    if(auto aim = std::get_if<Aim>(&action)) {
      player->direction = aim->direction;
    }

    if(auto move = std::get_if<Move>(&action)) {
      glm::vec2 playerPos = player->position;

      playerPos.x = playerPos.x + move->x * player->speed;
      playerPos.y = playerPos.y + move->y * player->speed;

      player->position = playerPos;
    }   
  }

  // process ai movement
  moveZombies();
  moveHumans();

  for(int i = 0; i < humans.size(); i++) {
    if(humans[i]->health <= 0) {
      // delete human
      delete humans[i];
      humans[i] = humans.back();
      humans.pop_back();
    }
  }

  // process ai collision
  for(int i = 0; i < humans.size(); i++) {
    // check if colliding with other fellow humans
    isCollidingEntities(humans[i], humans, i + 1);

    // check if colliding with wall
    isCollidingWall(humans[i]->position, TILE_SCALE);

    if(isCollidingEntities(humans[i], zombies, 0)) {
      if(humans[i] == player) {
        gameState = GameState::GAMOVER;
          break;
      }
      // create new zombie
      zombies.emplace_back(new Entity{.position = humans[i]->position,
        .direction =humans[i]->direction,
        .health = 10.0f, .speed = 1.0f});

      // delete human
      delete humans[i];
      humans[i] = humans.back();
      humans.pop_back();
    }
  }

  for(int i = 0; i < zombies.size(); i++) {
    if(zombies[i]->health <= 0) {
      // delete human
      delete zombies[i];
      zombies[i] = zombies.back();
      zombies.pop_back();
    }
  }

  //check victory condition
  if(zombies.size() == 0) {
    //todo: create victory texture
    gameState = GameState::VICTORY;
  }
  
  // process zombie collision
  for(int i = 0; i < zombies.size(); i++) {
     isCollidingEntities(zombies[i], zombies, i + 1);     
     isCollidingWall(zombies[i]->position, TILE_SCALE);
  }
}

void ZombieGame::isCollidingWall(glm::vec2& entityPosition, float scale) {
  glm::vec2 upperLeft = glm::vec2(entityPosition.x, entityPosition.y); 
  glm::vec2 upperRight = glm::vec2(entityPosition.x + scale, entityPosition.y); 
  glm::vec2 lowerLeft = glm::vec2(entityPosition.x, entityPosition.y + scale); 
  glm::vec2 lowerRight = glm::vec2(entityPosition.x + scale, entityPosition.y + scale); 

  // get position of the tile in map-space
  collideWithTile(upperLeft, entityPosition);
  collideWithTile(upperRight, entityPosition);
  collideWithTile(lowerLeft, entityPosition);
  collideWithTile(lowerRight, entityPosition);
  
  return;
}

void ZombieGame::collideWithTile(const glm::vec2& cornerPosition, glm::vec2& entityPosistion) {
  glm::vec2 tilePos = glm::vec2(floor(cornerPosition.x / TILE_SCALE), floor(cornerPosition.y / TILE_SCALE));

  if(levelData[tilePos.y][tilePos.x] != '.') {
    glm::vec2 tileCenter = tilePos * TILE_SCALE + glm::vec2(TILE_SCALE / 2);

    const float TILE_RADIUS = (float)TILE_SCALE / 2.0f;
    const float MIN_DISTANCE = TILE_RADIUS * 2;

    glm::vec2 centerAgentPos = entityPosistion + glm::vec2(TILE_RADIUS);
    glm::vec2 distVec = centerAgentPos - tileCenter;

    float xDepth = MIN_DISTANCE - abs(distVec.x);
    float yDepth = MIN_DISTANCE - abs(distVec.y);

    if (xDepth > 0 && yDepth > 0) {

        // Check which collision depth is less
        if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
            // X collsion depth is smaller so we push in X direction
            if (distVec.x < 0) {
                entityPosistion.x = entityPosistion.x - xDepth;
            } else {
                entityPosistion.x = entityPosistion.x + xDepth;
            }
        } else {
            // Y collsion depth is smaller so we push in X direction
            if (distVec.y < 0) {
                entityPosistion.y = entityPosistion.y - yDepth;
            } else {
                entityPosistion.y = entityPosistion.y + yDepth;
            }
        }
    }
  }
}

bool ZombieGame::isBulletCollidingWithEntity(const glm::vec2& bulletPos, Entity* entity) {
    const float MIN_DISTANCE = (TILE_SCALE / 2) + (BULLET_SCALE / 2);

    glm::vec2 distVec = bulletPos - (entity->position + (TILE_SCALE / 2));

    float distance = glm::length(distVec);

    float collisionDepth = MIN_DISTANCE - distance;
    if (collisionDepth > 0) { 
        entity->health -= 2;
        return true;
    }
  return false;
}

bool ZombieGame::isBulletCollidingWithWorld(const glm::vec2& bulletPos) {
  glm::vec2 tilePos = glm::vec2(floor(bulletPos.x / TILE_SCALE), floor(bulletPos.y / TILE_SCALE));

  if(levelData[tilePos.y][tilePos.x] != '.') {    
    return true;
  }
  return false;
}

void ZombieGame::moveZombies() {
  // track closest player and follow him
  for(auto zombie : zombies) {

    auto closestHuman = findClosestHuman(zombie);
    if(closestHuman.has_value()) {
      //calculate direction to this human
      Entity* human = closestHuman.value();
      zombie->direction = glm::normalize(human->position - zombie->position);
      zombie->position = zombie->position + zombie->direction * zombie->speed;
    }
  }
}

 std::optional<Entity*> ZombieGame::findClosestHuman(Entity* zombie) {    
  float minDistance = 999999;
  Entity* minHuman = nullptr;
  for(auto human : humans) {
      auto direction = zombie->position - human->position;
      auto distance = glm::length(direction);

      if(distance < minDistance) {
        minDistance = distance;
        minHuman = human;
      }
    }
  return minHuman ? std::optional<Entity*>(minHuman) : std::nullopt;
}

void ZombieGame::moveHumans() {
  for(auto human : humans) {
    if(human == player) {
      continue;
    }
    human->position = human->position + human->direction * human->speed;

    int chance = rand() % 500 + 1;

    if(chance <= 5) {
      float degree = rand() % 360 + 1;
      human->direction = glm::rotate(human->direction, degree);
    }
  }
}

void ZombieGame::resetParams() {
  for(auto zombie : zombies) {
    delete zombie;
  }
  zombies.clear();

  for(auto human : humans) {
    delete human;
  }
  humans.clear();

  levelData.clear();
  levelHeight = 0;
  levelWidth = 0;
}

bool ZombieGame::isCollidingEntities(Entity* entity, std::vector<Entity*>& entities, int i) {
  // loop through all humans and zombies and do that simple calculation
  for(int j = i; j < entities.size(); j++) {

    const float AGENT_RADIUS = TILE_SCALE / 2;
    // Center position of this agent
    glm::vec2 centerPosA = entity->position + glm::vec2(AGENT_RADIUS);
    // Center position of the parameter agent
    glm::vec2 centerPosB = entities[j]->position + glm::vec2(AGENT_RADIUS);

    // Distance vector between the two agents
    glm::vec2 distVec = centerPosA - centerPosB;
    
    // Length of the distance vector
    float distance = glm::length(distVec);

    // Depth of the collision
    float collisionDepth = TILE_SCALE - distance;

    // If collision depth > 0 then we did collide
    if (collisionDepth > 0) {

        // Get the direction times the collision depth so we can push them away from each other
        glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
        auto result = entity->position + collisionDepthVec / 2.0f;

        // Push them in opposite directions
        entity->position = entity->position + collisionDepthVec / 2.0f;
        entities[j]->position = entities[j]->position - collisionDepthVec / 2.0f;

        return true;
    }
  }
  return false;
}

void
ZombieGame::loadLevel()
{
  std::string level = levels[levelIndex];
  std::ifstream input(level);

  if(!input.is_open()) {
    throw std::runtime_error("Failed to load level: " + level);
  }

  int y = 0;
  // populate the level
  for(std::string line; std::getline(input, line);) {
    levelHeight++;

    if(levelWidth == 0) {
      levelWidth = line.length();
    }
    
    for(int x = 0; x < levelWidth; x++) {
      switch (line[x]) {
      case '#':
        break;
      case '@':
        player = new Entity(glm::vec2(x * TILE_SCALE, y * TILE_SCALE), glm::vec2(0.0f, 0.0f), 10.0f, 5.0f);
        humans.insert(humans.begin(), player);
        line[x] = '.';
        break;
      case 'H':
        humans.emplace_back(new Entity{.position = glm::vec2(x * TILE_SCALE, y * TILE_SCALE),
          .direction = glm::vec2(1.0, 0.0),
          .health = 10.0f, .speed = 4.0f});
        line[x] = '.';
        break;
      case 'Z':
        zombies.emplace_back(new Entity{.position = glm::vec2(x * TILE_SCALE, y * TILE_SCALE),
          .direction = glm::vec2(1.0, 0.0),
          .health = 10.0f, .speed = 1.0f});
        line[x] = '.';
        break;
     case '.':
        break;
      default:
        throw std::runtime_error("Unsupported char in " + line + " in level " + level); 
        break;
      }
    }

    levelData.push_back(line);
    y++;
  }

  gameState = GameState::RUNNING;
}
