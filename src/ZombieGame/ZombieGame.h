#ifndef _ZOMBIE_GAME_H_
#define _ZOMBIE_GAME_H_

#include <string>
#include <vector>
#include <optional>
#include "Entity.h"
#include "Bullet.h"
#include "../Actions.h"

enum GameState {
  UNINITIALIZED,
  INITIALIZED,
  RUNNING,
  PAUSED,
  GAMOVER,
  VICTORY,
};

class ZombieGame
{
public:
  ZombieGame();
  ~ZombieGame();

  void update(const std::vector<Actions>& actions);
  void loadLevel();

  const GameState& getGameState() const { return gameState; }
  const glm::vec2& getPlayerPos() const { return player->position; }
  const std::vector<std::string>& getLevelData() const { return levelData; }
  const float getTileScale() const { return TILE_SCALE; }
  const unsigned int getNumBullets() const { return MAX_NUM_BULLETS; }
  const float getBulletScale() const { return BULLET_SCALE; }
  const std::vector<Entity*>& getHumans() const { return humans; } 
  const std::vector<Entity*>& getZombies() const { return zombies; } 
  const std::vector<Bullet*>& getBullets() const { return bullets; }
  //let us try to write some code at the speed of poo with the new keyboard.

private:

  bool isBulletCollidingWithWorld(const glm::vec2& bulletPos);
  bool isBulletCollidingWithEntity(const glm::vec2& bulletPos, Entity* entity);
  void isCollidingWall(glm::vec2& entityPosition, float scale);
  void collideWithTile(const glm::vec2& cornerPosition, glm::vec2& entityPosition);
  bool isCollidingEntities(Entity* entity, std::vector<Entity*>& entities, int startIndex);
  bool isCollidngBullets(Bullet* bullet, std::vector<Entity*>& entities);

  void moveZombies();
  std::optional<Entity*> findClosestHuman(Entity* zombie);
  void moveHumans();

  void runGameLogic(const std::vector<Actions>& actions);
  void resetParams();

  const unsigned int MAX_NUM_BULLETS = 20;
  const float TILE_SCALE = 50.0f;
  const float BULLET_SCALE = 10.0f;

  uint32_t levelIndex;
  uint32_t levelWidth;
  uint32_t levelHeight;

  std::vector<std::string> levels;
  std::vector<std::string> levelData;

  GameState gameState;

  Entity* player;
  std::vector<Entity*> humans;
  std::vector<Entity*> zombies;
  std::vector<Bullet*> bullets;
};

#endif
