/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/Constants.hpp"
#include "indiestudio/common/Scheduler.hpp"
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
    {
        srand(std::time(nullptr));
    }

    static irr::core::vector3df tryMove(irr::scene::ISceneNode *node, const irr::core::vector3df &vector, const irr::core::vector3df &velocity)
    {
        irr::core::vector3df actPos(node->getAbsolutePosition());

        if (vector.X == -1) {
            node->setRotation(irr::core::vector3df(0, 90, 0));
        } else if (vector.X == 1) {
            node->setRotation(irr::core::vector3df(0, 270, 0));
        } else if (vector.Z == 1) {
            node->setRotation(irr::core::vector3df(0, 180, 0));
        } else if (vector.Z == -1) {
            node->setRotation(irr::core::vector3df(0, 0, 0));
        }
        irr::core::vector3df newPos(
            actPos.X + (vector.X * velocity.X),
            actPos.Y + (vector.Y * velocity.Y),
            actPos.Z + (vector.Z * velocity.Z));
        //TODO: check boundingbox at newPos, if can't move return actPos
        node->setPosition(newPos);
        return newPos;
    }

    void World::move(const irr::core::vector3df &direction, ECS::Position &pos, ECS::Speed &speed, ECS::Node &node)
    {
        irr::core::vector3df newPos(
            tryMove(node.node, direction, irr::core::vector3df(speed.x, speed.y, speed.z))
        );
        pos.x = newPos.X;
        pos.y = newPos.Y;
        pos.z = newPos.Z;
    }

    void World::initParticle(WorldManager &manager, irr::scene::ISceneManager *scenemg)
    {
        (void)manager;
        auto &particle = ecs.addEntity();

        auto ps = scenemg->addParticleSystemSceneNode(false);
        auto emiter = ps->createSphereEmitter({0, 0, 0}, 100, {0 , 0.3F, 0}, 5, 10, {255, 0, 0, 0}, {255, 255, 255, 255}, 2000, 4000);
        ps->setEmitter(emiter);
        emiter->drop();
        auto fo = ps->createFadeOutParticleAffector();
        ps->addAffector(fo);
        fo->drop();

        ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        ecs.setComponent(particle, Particle(ps));
        ecs.setComponent(particle, MaterialTexture(0, "assets/textures/fire.bmp"));
        ecs.setComponent(particle, Scale(2, 2, 2));
        ecs.setComponent(particle, Position(30, 100, 23));
        ecs.setComponent(particle, Setup());

    }


    void World::initPlayer(WorldManager &manager, irr::scene::ISceneManager *scenemg, int playerId)
    {
        (void) manager;

        auto &player = ecs.addEntity();
        std::pair<short, short> positions[4] = {
            MapPattern::positionToTile(1, this->settings.height - 1),
            MapPattern::positionToTile(this->settings.width - 1, this->settings.height - 1),
            MapPattern::positionToTile(1, 1),
            MapPattern::positionToTile(1, 1)
        };

        auto node_p = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/player.md3"));
        ecs.setComponent(player, Node(node_p));
        ecs.setComponent(player, MaterialTexture(0, "assets/textures/player_" + Constants::PLAYER_COLORS[playerId] + ".png"));
        ecs.setComponent(player, MaterialFlag(irr::video::EMF_LIGHTING, false));
        ecs.setComponent(player, Scale(20, 20, 20));
        ecs.setComponent(player, Position(positions[playerId].first, 70, positions[playerId].second));
        ecs.setComponent(player, Speed(1, 1, 1));
        ecs.setComponent(player, Movement());
        auto animator = scenemg->createCollisionResponseAnimator(this->meta, node_p, {5, 5, 5}, {0, 0, 0});
        node_p->addAnimator(animator);
        animator->drop();

        auto eventCB = EventCallbacks<WorldECS>();
        IndieStudio::ECS::Event::EventData event;
        event.type = ECS::Event::EventType::INDIE_KEYBOARD_EVENT;

        auto &pos = ecs.getComponent<Position>(player);
        auto &speed = ecs.getComponent<Speed>(player);
        auto &nodeEcs = ecs.getComponent<Node>(player);
        auto &mov = ecs.getComponent<Movement>(player);

        if (this->settings.players[playerId].controlType == WorldSettings::Player::ControlType::KEYBOARD) {
            event.keyInput.Key = this->settings.players[playerId].keyboardUp;
            eventCB.addCallback(event,
                [&] (const EventData &event, std::size_t id, WorldECS &ecs)
                {
                    if (!event.keyInput.PressedDown) {
                        mov.up = event.keyInput.PressedDown;
                        return;
                    }
                    mov.up = event.keyInput.PressedDown;
                    mov.down = false;
                    mov.left = false;
                    mov.right = false;
                });
            event.keyInput.Key = this->settings.players[playerId].keyboardDown;
            eventCB.addCallback(event,
                [&] (const EventData &event, std::size_t id, WorldECS &ecs)
                {
                    if (!event.keyInput.PressedDown) {
                        mov.down = event.keyInput.PressedDown;
                        return;
                    }
                    mov.down = event.keyInput.PressedDown;
                    mov.up = false;
                    mov.left = false;
                    mov.right = false;
                });
            event.keyInput.Key = this->settings.players[playerId].keyboardLeft;
            eventCB.addCallback(event,
                [&] (const EventData &event, std::size_t id, WorldECS &ecs)
                {
                    if (!event.keyInput.PressedDown) {
                        mov.left = event.keyInput.PressedDown;
                        return;
                    }
                    mov.left = event.keyInput.PressedDown;
                    mov.up = false;
                    mov.down = false;
                    mov.right = false;
                });
            event.keyInput.Key = this->settings.players[playerId].keyboardRight;
            eventCB.addCallback(event,
                [&] (const EventData &event, std::size_t id, WorldECS &ecs)
                {
                    if (!event.keyInput.PressedDown) {
                        mov.right = event.keyInput.PressedDown;
                        return;
                    }
                    mov.right = event.keyInput.PressedDown;
                    mov.up = false;
                    mov.down = false;
                    mov.left = false;
                });
        }

        ecs.setComponent(player, eventCB);
        ecs.setComponent(player, Setup());
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
        node->addShadowVolumeSceneNode();
        node->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setScale(irr::core::vector3df(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
		node->setPosition(irr::core::vector3df(0.5, 50, 0.5));
        node->addShadowVolumeSceneNode(node->getMesh());

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

        auto &powerup = ecs.addEntity();

        ecs.setComponent(powerup, Node(Node(static_cast<irr::scene::IAnimatedMeshSceneNode *>(node->clone()))));
        ecs.setComponent(powerup, MaterialTexture(0, "assets/textures/tmp_powerUp.png"));
        ecs.setComponent(powerup, MaterialFlag(irr::video::EMF_LIGHTING, true));
        ecs.setComponent(powerup, Scale(9, 9, 9));
        ecs.setComponent(powerup, Position(28, 70, 45));
        ecs.setComponent(powerup, IsPowerUp());
        ecs.setComponent(powerup, Setup());

        generator->generate(this->pattern.get());
	    this->pattern->forEach([&](int x, int y, int z, MapPattern::TileType tileType) {
	    	if (tileType == MapPattern::TileType::EMPTY)
	    		return;

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
                ecs.getComponent<Node>(newBlock).node->addShadowVolumeSceneNode();
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_1.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
	    	} else if (tileType == MapPattern::TileType::FLOOR_SECOND) {
                ecs.getComponent<Node>(newBlock).node->addShadowVolumeSceneNode();
	    	    ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_2.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
	    	} else if (tileType == MapPattern::TileType::BORDER_WALL_BLOCK) {
                ecs.getComponent<Node>(newBlock).node->addShadowVolumeSceneNode();
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
	    	} else if (tileType == MapPattern::TileType::INNER_WALL_BLOCK) {
                ecs.getComponent<Node>(newBlock).node->addShadowVolumeSceneNode();
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR * 0.9, Constants::TILE_SIZE_FACTOR * 0.9, Constants::TILE_SIZE_FACTOR * 0.9));
            } else if (tileType == MapPattern::TileType::BREAKABLE_BLOCK) {
                ecs.getComponent<Node>(newBlock).node->addShadowVolumeSceneNode();
	    		ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_brick.png"));
                ecs.setComponent(newBlock, Alive());
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR * 0.9, Constants::TILE_SIZE_FACTOR * 0.9, Constants::TILE_SIZE_FACTOR * 0.9));
                ecs.setComponent(newBlock, Alive());
            }

            ecs.setComponent(newBlock, MaterialFlag(irr::video::EMF_LIGHTING, true));
            ecs.setComponent(newBlock, Setup());
        });

        initPlayer(manager, scenemg, 0);
        initPlayer(manager, scenemg, 1);
        initPlayer(manager, scenemg, 2);
        initPlayer(manager, scenemg, 3);

        initParticle(manager, scenemg);

        Initializer<WorldECS>::initAllEntities(ecs, scenemg);
    }

    void World::focusECS(SceneManager::Scene &scene)
    {
        auto systems = WorldECSSystems(this->ecs, this);

        while (Game::INSTANCE->getSceneManager().getActive() == SceneManager::PLAY_ID
        && Singleton::getDevice()->run()) {
            this->ecs.getEventManager().switch_event_queue();
            scene.scene->getVideoDriver()->beginScene(true, true);
            systems.process();
            this->ecs.forEntitiesWith<Movement>(
                [&](auto &data, [[gnu::unused]] std::size_t id)
                {
                    if (settings.elapsedSeconds == 0)
                        return;

                    Movement &mov = this->ecs.getComponent<Movement>(data);
                    auto &speed = this->ecs.getComponent<Speed>(data);
                    auto &pos = this->ecs.getComponent<Position>(data);
                    auto &node = this->ecs.getComponent<Node>(data);

                    bool did = false;
                    if (mov.up) {
                        move(direction[0], pos, speed, node);
                        did = true;
                    } if (mov.down) {
                        move(direction[1], pos, speed, node);
                        did = true;
                    } if (mov.left) {
                        move(direction[2], pos, speed, node);
                        did = true;
                    } if (mov.right) {
                        move(direction[3], pos, speed, node);
                        did = true;
                    }
                    if (!did) {
                        if (node.node->getFrameNr() == 76 || node.node->getFrameNr() <= 27)
                            node.node->setFrameLoop(27, 76);
                        return;
                    }
                    if (node.node->getFrameNr() > 27)
		                node.node->setFrameLoop(0, 27);
                }
            );
            this->ecs.getEventManager().clear_event_queue();
            scene.scene->drawAll();
            scene.gui->draw();
            scene.scene->getVideoDriver()->endScene();
            Scheduler::tick();
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