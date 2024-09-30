#ifndef _GAME_H_
#define _GAME_H_

#include "utils.h"
#include "entity.h"

#include <_types/_uint32_t.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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

  const std::vector<DynamicUBO> getObjectData() const { return objectData; }

private:
  void loadLevel(uint32_t levelIndex);
  void initEntities(uint32_t numHumans, uint32_t numZombies);

  GameState gameState;

  uint32_t levelIndex;
  uint32_t levelWidth;
  uint32_t levelHeight;

  std::vector<DynamicUBO> objectData;

  std::vector<std::string> levels;
  std::vector<char> levelData;

  // TODO: 2 vectors
  std::vector<Entity> humans;
  std::vector<Entity> zombies;
  // humans
  // zombies
  // player
};

#endif
