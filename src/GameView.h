#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_

#include "Input/IInputManager.h"
#include "vulkan_renderer.h"
#include "Actions.h"
#include "ZombieGame/ZombieGame.h"
#include "Camera.h"

#include <memory>

class GameView {
public:
  GameView(std::unique_ptr<VulkanRenderer>& renderer, const ZombieGame& game, const Camera& camera); 
  ~GameView();

  const std::vector<Actions>& processInput(const std::span<const InputEvent> inputEvents);
  void drawBackground();
  void draw();

private:
  void drawEntities();
  size_t entitiesSize;
  std::vector<InstanceData> instanceData;
 
  std::unique_ptr<VulkanRenderer>& renderer;

  std::vector<Actions> actions;

  const ZombieGame& game;
  glm::mat4 cameraMatrix;
  Camera camera;
  
  std::vector<std::pair<VkBuffer, size_t>> renderData;
  bool readyToDraw;
  
  VkBuffer levelInstanceBuffer;
  VmaAllocation levelBufferAllocation;

  VkBuffer entitiesInstanceBuffer;
  VmaAllocation entitiesBufferAllocation;
};

#endif
