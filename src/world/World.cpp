/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/ecs/Events.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    World::World(WorldSettings settings)
        : settings(settings),
        scene(Game::INSTANCE->getSceneManager().getScene(SceneManager::PLAY_ID))
    {}

    World::World() : World(WorldSettings()),
        scene(Game::INSTANCE->getSceneManager().getScene(SceneManager::PLAY_ID))
    {}

    void World::create(WorldManager &manager)
    {
        MapPattern pattern(this->settings.width, this->settings.height);
        IWorldGenerator *generator = nullptr;

        for (auto it = manager.getGenerators().begin(); it != manager.getGenerators().end(); ++it) {
            if (it->first == this->settings.worldGenerator) {
                generator = it->second;
                break;
            }
        }

        if (generator == nullptr)
            throw std::runtime_error(
                "Failed to find the world generator " + this->settings.worldGenerator);
        
        generator->generate(pattern);

        // TODO: Fill the ECS with the entities
    }

    void World::focusECS(irr::scene::ISceneManager *sceneManager)
    {
        auto systems = WorldECSSystems(this->ecs);

        while (Game::INSTANCE->getSceneManager().getActive() == SceneManager::PLAY_ID
        && Game::getDevice()->run()) {
            sceneManager->getVideoDriver()->beginScene(true, true);
            systems.process();
            this->ecs.getEventManager().clear_event_queue();
            sceneManager->drawAll();
            sceneManager->getVideoDriver()->endScene();
        }
    }

    void World::forwardEvent(ECS::Event::EventData &event)
    {
        this->ecs.getEventManager().push_event(event);
    }

    void World::pack(ByteBuffer &buffer) const
    {
        this->settings.pack(buffer);
    }

    void World::unpack(ByteBuffer &buffer)
    {
        this->settings.unpack(buffer);

        // TODO: Restore the entities in the ECS
    }

}