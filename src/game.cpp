#include "game.h"
#include "utils.h"
#include <_types/_uint32_t.h>

Game::Game()
{
  levels.push_back("../levels/level1");
  assert(levels.size() > 0);

  objectData.reserve(100);
  levelData.reserve(100);

  levelHeight = 0;
  levelWidth = 0;
  
  gameState = GameState::UNINITIALIZED;

  levelIndex = 0;
}

Game::~Game() {}

void
Game::update()
{
  if (gameState == GameState::UNINITIALIZED ||
      gameState == GameState::FINISHED) {
    loadLevel(levelIndex);

    gameState = GameState::RUNNING;
    levelIndex++;
  }
}

void
Game::loadLevel(uint32_t levelIndex)
{
  std::string level = levels[levelIndex];
  std::ifstream input(level);

  if(!input.is_open()) {
    throw std::runtime_error("Failed to load level: " + level);
  }

  uint32_t scale = 50;
  uint32_t numHumans, numZombies;

  // populate the level
  for(std::string line; std::getline(input, line);) {
    levelHeight++;

    if(levelWidth == 0) {
      levelWidth = line.length();
    }
    
    for(int x = 0; x < levelWidth; x++) {
      levelData.push_back(line[x]);
      DynamicUBO dUbo{};
      
      switch (line[x]) {
      case '#':
        dUbo.model =
        glm::translate(glm::mat4(1.0f), glm::vec3(x * scale, levelHeight * scale, 0));
        dUbo.model = glm::scale(dUbo.model, glm::vec3(scale, scale, 0));
        dUbo.array_index = 0;

        objectData.push_back(dUbo);
        break;
      case '@':
        //TODO: case for the player
        break;
      case 'H':
        numHumans++;
        
        break;
      case 'Z':
        numZombies++;

        break;
      case '.':

        break;
      default:
        throw std::runtime_error("Unsupported char in " + line + " in level " + level);
        break;
      }
    }
  }
}
