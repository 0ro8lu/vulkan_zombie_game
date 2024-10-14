#ifndef _GAME_H_
#define _GAME_H_

#include "entity.h"

#include <string>
#include <vector>

enum GameState
{
  UNINITIALIZED,
  FINISHED,
  RUNNING,
};

class Game
{
public:
  Game();
  ~Game();

  void update();

  const std::vector<glm::vec2>& getLevelSprites() const { return levelSprites; }
  const std::vector<Entity>& getHumans() const { return humans; }
  const std::vector<Entity>& getZombies() const { return zombies; }

  // const std::vector<DynamicUBO> getObjectData() const { return objectData; }

private:
  void loadLevel(uint32_t levelIndex);
  void initEntities(uint32_t numHumans, uint32_t numZombies);

  GameState gameState;

  uint32_t levelIndex;
  uint32_t levelWidth;
  uint32_t levelHeight;

  // std::vector<DynamicUBO> objectData;

  std::vector<std::string> levels;
  std::vector<char> levelData;

  std::vector<glm::vec2> levelSprites;
  std::vector<Entity> humans;
  std::vector<Entity> zombies;
};

#endif
