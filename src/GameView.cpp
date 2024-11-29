#include "GameView.h"
#include "Actions.h"
#include "Input/IInputManager.h"
#include "ZombieGame/Bullet.h"
#include "ZombieGame/ZombieGame.h"
#include "vulkan_renderer.h"
#include <utility>

GameView::GameView(std::unique_ptr<VulkanRenderer>& renderer, const ZombieGame& game, const Camera& camera)
        : renderer(renderer), game(game), readyToDraw(false), camera(camera){
    actions.reserve(10);
}

GameView::~GameView() {
    renderer->deallocateInstanceBuffer(levelInstanceBuffer, levelBufferAllocation);
    renderer->deallocateInstanceBuffer(entitiesInstanceBuffer, entitiesBufferAllocation);
}

const std::vector<Actions>& GameView::processInput(const std::span<const InputEvent> inputEvents) {
    actions.clear();
    
    short x, y = 0;
    glm::vec2 direction = glm::vec2(0.0, 0.0);
    
    for(const auto& iEvent : inputEvents) {

        // mouse click events
        if(iEvent.type == InputEvent::Type::MousePress) {
            auto mouseButton = std::get<MouseButton>(iEvent.data);
            if(mouseButton == MouseButton::LEFT) {
                actions.emplace_back(Shoot{});
            }
        }

        // mouse move events
        if(iEvent.type == InputEvent::Type::MouseMove) {
            auto tmp = std::get<std::pair<double,double>>(iEvent.data);
            auto mousePos = glm::vec2(tmp.first, tmp.second);
            mousePos = camera.convertScreenToWorld(mousePos);

            // calculate aiming angle
            direction = -glm::normalize((game.getPlayerPos() + game.getTileScale()) - mousePos);            
        }

        // keyboard events
        if(iEvent.type == InputEvent::Type::KeyPress || iEvent.type == InputEvent::Type::KeyRepeat) {
            if(std::get<KeyCode>(iEvent.data) == KeyCode::RETURN && (game.getGameState() == GameState::VICTORY || game.getGameState() == GameState::GAMOVER)) {
                actions.emplace_back(LoadLevel{});
            }
            if(std::get<KeyCode>(iEvent.data) == KeyCode::W) {
                y = -1;
            }
            if(std::get<KeyCode>(iEvent.data) == KeyCode::S) {
                y = 1;
            }
            if(std::get<KeyCode>(iEvent.data) == KeyCode::A) {
                x = -1;
            }
            if(std::get<KeyCode>(iEvent.data) == KeyCode::D) {
                x = 1;
            }
        }
    }

    if(x != 0 || y != 0 ){
        actions.emplace_back(Move{x, y});
    }
    
    if(direction != glm::vec2(0.0f, 0.0f)) {
        actions.emplace_back(Aim {direction});
    }

    return actions;
}

void GameView::drawBackground() {    
    const std::vector<std::string>& levelData = game.getLevelData();
    float tileScale = game.getTileScale();
    instanceData.clear();
    renderData.clear();
    
    for(int y = 0; y < levelData.size(); y++) {
        for(int x = 0; x < levelData[y].size(); x++) {
            glm::mat4 model = glm::mat4(1.0f);

            switch (levelData[y][x]) {
                case '#':
                    model = glm::translate(glm::mat4(1.0f), glm::vec3(x * tileScale, y * tileScale, 0.0f));
                    model = glm::scale(model, glm::vec3(tileScale, tileScale, 0.0f));

                    instanceData.emplace_back(InstanceData{.model = model, .textureIndex = 0});
                    
                    break;
                //TODO: insert more wall types.
            }
        }
    }

    renderer->allocateInstanceBuffer(instanceData.size(), levelInstanceBuffer, levelBufferAllocation);
    renderer->uploadToInstanceBuffer(levelInstanceBuffer, instanceData);
    renderData.push_back(std::make_pair(levelInstanceBuffer, instanceData.size()));

    //game view is always called after game, so we know the game has already initialized its data, so we can read it.
    entitiesSize = game.getHumans().size() + game.getZombies().size() + 1 + game.getNumBullets(); // the + 1 accomodates data for the gameover texture
    renderer->allocateInstanceBuffer(entitiesSize, entitiesInstanceBuffer, entitiesBufferAllocation);
    renderData.push_back(std::make_pair(entitiesInstanceBuffer, entitiesSize));

    drawEntities();
    readyToDraw = true;
}

void GameView::drawEntities() {

    instanceData.clear();

    if(game.getGameState() == GameState::GAMOVER) {
        auto lookAt = camera.getLookAt();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lookAt.x, lookAt.y, lookAt.z));
        model = glm::scale(model, glm::vec3(-800, 800, 0.0f));

        instanceData.emplace_back(InstanceData{.model = model, .textureIndex = 3});
    } else if(game.getGameState() == GameState::VICTORY){
        auto lookAt = camera.getLookAt();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lookAt.x, lookAt.y, lookAt.z));
        model = glm::scale(model, glm::vec3(-800, 800, 0.0f));

        instanceData.emplace_back(InstanceData{.model = model, .textureIndex = 4});
    }
    else if(game.getGameState() == GameState::RUNNING){

        const std::vector<Entity*>& humans = game.getHumans();
        const std::vector<Entity*>& zombies = game.getZombies();
        const std::vector<Bullet*>& bullets = game.getBullets();
        float tileScale = game.getTileScale();
        float bulletScale = game.getBulletScale();

        for(const auto& human : humans) {

            const glm::vec2 right(-1.0f, 0.0f);
            float angle = -acos(glm::dot(right, human->direction));
            if (human->direction.y < 0.0f) angle = -angle;
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(human->position.x, human->position.y, 0.0f));
            model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(tileScale, tileScale, 0.0f));

            instanceData.emplace_back(InstanceData{.model = model, .textureIndex = 1});
        }

        for(const auto& zombie : zombies) {
            const glm::vec2 right(-1.0f, 0.0f);
            float angle = -acos(glm::dot(right, zombie->direction));
            if (zombie->direction.y < 0.0f) angle = -angle;
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(zombie->position.x, zombie->position.y, 0.0f));
            model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(tileScale, tileScale, 0.0f));

            instanceData.emplace_back(InstanceData{.model = model, .textureIndex = 2});
        }

        // draw bullets
        for(const auto& bullet : bullets) {

            glm::mat4 model = glm::mat4(1.0f);
            // i dunno why the renderer would draw the bullets in the middle of a tile. this fixes it ^^
            model = glm::translate(model, glm::vec3(bullet->position.x - tileScale / 2, bullet->position.y - tileScale / 2, 0.0f));
            model = glm::scale(model, glm::vec3(bulletScale, bulletScale, 0.0f));

            instanceData.emplace_back(InstanceData{.model = model, .textureIndex = 2});
        }
    }

    if(instanceData.size() == 0) {
        return;
    }

    renderData.pop_back();
    renderData.push_back(std::make_pair(entitiesInstanceBuffer, instanceData.size()));

    renderer->uploadToInstanceBuffer(entitiesInstanceBuffer, instanceData);
}

void GameView::draw() {

    if(game.getGameState() == GameState::UNINITIALIZED){ readyToDraw = false; }

    if(readyToDraw) {
        //draw the entities
        drawEntities();
    } else {        
	    renderer->wait();
        renderer->deallocateInstanceBuffer(levelInstanceBuffer, levelBufferAllocation);
        renderer->deallocateInstanceBuffer(entitiesInstanceBuffer, entitiesBufferAllocation);
        return;
    }
    
    //update the camera based on player position
    auto playerPos = game.getPlayerPos();
    camera.setPosition(playerPos);
  
    renderer->draw(renderData, camera.getCameraMatrix());
}
