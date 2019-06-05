/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/ecs/Events.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/world/WorldManager.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/ecs/Initializer.hpp"

namespace IndieStudio {

    World::World(WorldSettings settings)
        : settings(settings),
        pattern(std::make_unique<MapPattern>(settings.width, settings.height)),
        scene(Game::INSTANCE->getSceneManager().getScene(SceneManager::PLAY_ID)),
        meta(this->scene.scene->createMetaTriangleSelector())
    {}

    World::World() : World(WorldSettings())
    {}

    static irr::core::vector3df try_move(irr::scene::ISceneNode *node, const irr::core::vector3df &vector, const irr::core::vector3df &velocity)
    {
        irr::core::vector3df actPos(node->getAbsolutePosition());
        irr::core::vector3df newPos(
            actPos.X + (vector.X * velocity.X),
            actPos.Y + (vector.Y * velocity.Y),
            actPos.Z + (vector.Z * velocity.Z));
        //TODO: check boundingbox at newPos, if can't move return actPos
        node->setPosition(newPos);
        return newPos;
    }

    std::function<void(const ECS::EventData&, std::size_t, WorldECS &)> World::move(
        const irr::core::vector3df &direction, ECS::Position &pos, ECS::Speed &speed, ECS::Node &node)
    {
        return [&] (const EventData &event, std::size_t id, WorldECS &ecs)
            {
                irr::core::vector3df newPos(
                    try_move(node.node, direction, irr::core::vector3df(speed.x, speed.y, speed.z))
                );
                pos.x = newPos.X;
                pos.y = newPos.Y;
                pos.z = newPos.Z;
            };
    }

    void World::create(WorldManager &manager)
    {
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
		ecs.setComponent(bomb, Position(40.5, 70, 100.5));
        ecs.setComponent(bomb, IsBomb());
        ecs.setComponent(bomb, ExplosionRange());
        ecs.setComponent(bomb, LifeTime());
        ecs.setComponent(bomb, Setup());

        generator->generate(this->pattern.get());
	    this->pattern->forEach([&](int x, int y, int z, MapPattern::TileType tileType) {
	    	if (tileType == MapPattern::TileType::EMPTY)
	    		return;

            // auto animator = scenemg->createCollisionResponseAnimator(meta, player1, {1, 1, 1}, {0, 0, 0});
            // player1->addAnimator(animator)
            // animator->drop();

            auto &newBlock = ecs.addEntity();
            ecs.setComponent(newBlock, Node(static_cast<irr::scene::IAnimatedMeshSceneNode *>(node->clone())));
            auto node = ecs.getComponent<Node>(newBlock).node;
            auto selector = scenemg->createTriangleSelectorFromBoundingBox(node);
            node->setTriangleSelector(selector);
            this->meta->addTriangleSelector(selector);

            ecs.setComponent(newBlock, Position(
                node->getPosition().X + node->getScale().X * x,
                node->getPosition().Y + node->getScale().Y * (y == 1 ? 1 : 0),
                node->getPosition().Z + node->getScale().Z * z
            ));

	    	if (tileType == MapPattern::TileType::FLOOR_FIRST) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_1.png"));
                ecs.setComponent(newBlock, Scale(20.0, 20.0, 20.0));
	    	} else if (tileType == MapPattern::TileType::FLOOR_SECOND) {
	    	    ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_2.png"));
                ecs.setComponent(newBlock, Scale(20.0, 20.0, 20.0));
	    	} else if (tileType == MapPattern::TileType::BORDER_WALL_BLOCK) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(20.0, 20.0, 20.0));
	    	} else if (tileType == MapPattern::TileType::INNER_WALL_BLOCK) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(20.0 * 0.9, 20.0 * 0.9, 20.0 * 0.9));
            } else if (tileType == MapPattern::TileType::BREAKABLE_BLOCK) {
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_brick.png"));
                ecs.setComponent(newBlock, Scale(20.0 * 0.9, 20.0 * 0.9, 20.0 * 0.9));
                ecs.setComponent(newBlock, Alive());
            }

            ecs.setComponent(newBlock, MaterialFlag(irr::video::EMF_LIGHTING, true));
            ecs.setComponent(newBlock, Setup());
        });

        auto &player1 = ecs.addEntity();

        auto node_p = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/player.md3"));
        ecs.setComponent(player1, Node(node_p));
	    ecs.setComponent(player1, MaterialTexture(0, "assets/textures/player_black.png"));
		ecs.setComponent(player1, MaterialFlag(irr::video::EMF_LIGHTING, false));
		ecs.setComponent(player1, Scale(20, 20, 20));
		ecs.setComponent(player1, Position(30, 70, 23));
        ecs.setComponent(player1, Speed(1, 1, 1));

        auto eventCB = EventCallbacks<WorldECS>();
        IndieStudio::ECS::Event::EventData event;
		event.type = ECS::Event::EventType::INDIE_KEYBOARD_EVENT;
        event.keyInput.Key = irr::KEY_UP;

        auto &pos = ecs.getComponent<Position>(player1);
        auto &speed = ecs.getComponent<Speed>(player1);
        auto &nodeEcs = ecs.getComponent<Node>(player1);

        eventCB.addCallback(event, move(direction[0], pos, speed, nodeEcs));

        event.keyInput.Key = irr::KEY_DOWN;
        eventCB.addCallback(event, move(direction[1], pos, speed, nodeEcs));

        event.keyInput.Key = irr::KEY_LEFT;
        eventCB.addCallback(event, move(direction[2], pos, speed, nodeEcs));

        event.keyInput.Key = irr::KEY_RIGHT;
        eventCB.addCallback(event, move(direction[3], pos, speed, nodeEcs));

        // eventCB.addCallback(event,
        //     [&] (const EventData& event, std::size_t id, WorldECS &ecs)
        //     {
        //         auto &pos = ecs.getComponent<Position>(player1);
        //         auto &speed = ecs.getComponent<Speed>(player1);
        //         auto &node = ecs.getComponent<Node>(player1).node;
        //         irr::core::vector3df newPos(
        //             move_player(node, irr::core::vector3df(0, 0, 1), irr::core::vector3df(speed.x, speed.y, speed.z))
        //         );
        //         pos.x = newPos.X;
        //         pos.y = newPos.Y;
        //         pos.z = newPos.Z;
        //     });

        ecs.setComponent(player1, eventCB);
        ecs.setComponent(player1, Setup());

        Initializer<WorldECS>::initAllEntities(ecs, scenemg);
    }

    void World::focusECS(irr::scene::ISceneManager *sceneManager)
    {
        auto systems = WorldECSSystems(this->ecs, this);

        while (Game::INSTANCE->getSceneManager().getActive() == SceneManager::PLAY_ID
        && Singleton::getDevice()->run()) {
            this->ecs.getEventManager().switch_event_queue();
            sceneManager->getVideoDriver()->beginScene(true, true);
            systems.process();
            this->ecs.getEventManager().clear_event_queue();
            sceneManager->drawAll();
            sceneManager->getVideoDriver()->endScene();
        }
    }

    void World::forwardEvent(ECS::Event::EventData event)
    {
        this->ecs.getEventManager().push_event(event);
    }

    void World::pack(ByteBuffer &buffer) const
    {
        this->settings.pack(buffer);
        this->pattern->pack(buffer);
    }

    void World::unpack(ByteBuffer &buffer)
    {
        this->settings.unpack(buffer);
        this->pattern = std::make_unique<MapPattern>(this->settings.width,
            this->settings.height);
        this->pattern->unpack(buffer);

        // TODO: Restore the entities in the ECS
    }

}