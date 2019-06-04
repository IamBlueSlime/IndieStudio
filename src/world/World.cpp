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
#include "indiestudio/ecs/Initializer.hpp"

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
        auto scenemg = this->scene.scene;

        auto node = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/cube.obj"));
        node->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setScale(irr::core::vector3df(20.0, 20.0, 20.0));
		node->setPosition(irr::core::vector3df(0.5, 50, 0.5));

        auto &bomb = ecs.addEntity();

        auto node1 = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/bomb.obj"));
        ecs.setComponent(bomb, Node(node1));
        ecs.setComponent(bomb, MaterialTexture(0, "assets/textures/bomb.png"));
		ecs.setComponent(bomb, MaterialFlag(irr::video::EMF_LIGHTING, true));
		ecs.setComponent(bomb, Scale(3.5, 3.5, 3.5));
		ecs.setComponent(bomb, Position(30, 70, 23));
        ecs.setComponent(bomb, IsBomb());
        ecs.setComponent(bomb, LifeTime());
        ecs.setComponent(bomb, Setup());

        generator->generate(pattern);
	    pattern.forEach([&](int x, int y, int z, char c) {
	    	if (c == IndieStudio::MapPattern::EMPTY_TILE)
	    		return;
            auto &newBlock = ecs.addEntity();

            ecs.setComponent(newBlock, Node(static_cast<irr::scene::IAnimatedMeshSceneNode *>(node->clone())));
            ecs.setComponent(newBlock, Position(
                node->getPosition().X + node->getScale().X * x,
                node->getPosition().Y + node->getScale().Y * (y == 1 ? 1 : 0),
                node->getPosition().Z + node->getScale().Z * z
            ));
	    	if (c == IndieStudio::MapPattern::GROUND_FIRST_TILE) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_1.png"));
                ecs.setComponent(newBlock, Scale(20.0, 20.0, 20.0));
	    	} else if (c == IndieStudio::MapPattern::GROUND_SECOND_TILE) {
	    	    ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_2.png"));
                ecs.setComponent(newBlock, Scale(20.0, 20.0, 20.0));
	    	} else if (c == IndieStudio::MapPattern::BORDER_WALL_TILE) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(20.0, 20.0, 20.0));
	    	} else if (c == IndieStudio::MapPattern::INNER_WALL_TILE) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(20.0 * 0.9, 20.0 * 0.9, 20.0 * 0.9));
            } else if (c == IndieStudio::MapPattern::BREAKABLE_BLOCK_TILE) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_brick.png"));
                ecs.setComponent(newBlock, Scale(20.0 * 0.9, 20.0 * 0.9, 20.0 * 0.9));
            }
            ecs.setComponent(newBlock, MaterialFlag(irr::video::EMF_LIGHTING, true));
            ecs.setComponent(newBlock, Setup());
	    });
        IndieStudio::Initializer<WorldECS>::initAllEntities(ecs, scenemg);
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