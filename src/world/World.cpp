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
#include "indiestudio/Singleton.hpp"

namespace IndieStudio {

    World::World(WorldSettings settings)
        : settings(settings),
        scene(Game::INSTANCE->getSceneManager().getScene(SceneManager::PLAY_ID))
    {}

    World::World() : World(WorldSettings())
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

        auto driver = IndieStudio::Singleton::getDevice()->getVideoDriver();
        //TODO : get the right scene to draw inside
        auto scenemg = this->scene.scene;
        irr::scene::IAnimatedMeshSceneNode* node = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/cube.obj"));
        node->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setScale(irr::core::vector3df(20, 20, 20));
		node->setPosition(irr::core::vector3df(0.5, 50, 0.5));
        generator->generate(pattern);
        // TODO: Fill the ECS with the entities
	    pattern.forEach([&](int x, int y, int z, char c) {
            static int i = 0;
            std::cout << "Je do le bloc n°" << i++ << " en " << node->getPosition().X + node->getScale().X * x <<  " et " <<
            node->getPosition().Y + node->getScale().Y * (y == 1 ? 1 : 0) << " et " << node->getPosition().Z + node->getScale().Z * z << std::endl;
	    	if (c == IndieStudio::MapPattern::EMPTY_TILE)
	    		return;
	    	irr::scene::ISceneNode* tmp = node->clone();
	    	tmp->setPosition(irr::core::vector3df(
	    		node->getPosition().X + node->getScale().X * x,
	    		node->getPosition().Y + node->getScale().Y * (y == 1 ? 1 : 0),
	    		node->getPosition().Z + node->getScale().Z * z
	    	));
	    	if (c == IndieStudio::MapPattern::GROUND_FIRST_TILE) {
	    		tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
	    	} else if (c == IndieStudio::MapPattern::GROUND_SECOND_TILE) {
	    		tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_2.png"));
	    	} else if (c == IndieStudio::MapPattern::BORDER_WALL_TILE) {
	    		tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_wall.png"));
	    	} else if (c == IndieStudio::MapPattern::INNER_WALL_TILE) {
	    		tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_wall.png"));
	    		tmp->setScale(tmp->getScale() / 1.1);
	    	} else if (c == IndieStudio::MapPattern::BREAKABLE_BLOCK_TILE) {
	    		tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_brick.png"));
	    		tmp->setScale(tmp->getScale() / 1.1);
	    	}
	    });
    }

    void World::focusECS(irr::scene::ISceneManager *sceneManager)
    {
        auto systems = WorldECSSystems(this->ecs);

        while (Game::INSTANCE->getSceneManager().getActive() == SceneManager::PLAY_ID
        && Singleton::getDevice()->run()) {
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