#ifndef GAME_GRAPHICS_MANAGER_H_
#define GAME_GRAPHICS_MANAGER_H_

#include "vulkan_renderer.h"

#include <memory>

const float SPRITE_SCALE = 50.0f;

class GameGraphicsManager {
public:
  GameGraphicsManager(std::unique_ptr<VulkanRenderer>& renderer); 
  ~GameGraphicsManager();

  void draw(std::vector<glm::vec2> levelSprites);

private:
  bool firstTime;
  std::unique_ptr<VulkanRenderer>& renderer;

  // std::vector<std::pair<VkBuffer, VmaAllocation>> renderData;
  std::vector<std::pair<VkBuffer, size_t>> renderData;
  VkBuffer levelInstanceBuffer;
  VmaAllocation levelBufferAllocation;
};

#endif
