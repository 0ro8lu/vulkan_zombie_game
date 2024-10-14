#include "game_graphics_manager.h"
#include "vulkan_renderer.h"
#include <utility>
#include <iostream>

GameGraphicsManager::GameGraphicsManager(std::unique_ptr<VulkanRenderer>& renderer) : firstTime(true), renderer(renderer) { }

GameGraphicsManager::~GameGraphicsManager() {
    renderer->deallocateInstanceBuffer(levelInstanceBuffer, levelBufferAllocation);
}

void GameGraphicsManager::draw(std::vector<glm::vec2> levelSprites) {
    if(firstTime) {

        // Level geometry
        renderer->allocateInstanceBuffer(levelSprites.size(), levelInstanceBuffer, levelBufferAllocation);

        firstTime = false;
    }

    std::vector<InstanceData> levelInstanceData;
    levelInstanceData.reserve(levelSprites.size());

    for(int i = 0; i < levelSprites.size(); i++) {
        InstanceData instanceData {
            glm::vec3(levelSprites[i].x, levelSprites[i].y, 0),
            glm::vec3(0, 0, 0),
            SPRITE_SCALE,
            0,
        };
        
        levelInstanceData.push_back(instanceData);
    }
    
    renderer->uploadToInstanceBuffer(levelInstanceBuffer, levelInstanceData);

    renderData.push_back(std::make_pair(levelInstanceBuffer, levelSprites.size()));

    renderer->draw(renderData);
    // renderer->drawFrame(game.getObjectData());
}

