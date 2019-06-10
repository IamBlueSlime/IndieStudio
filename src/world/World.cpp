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
#include "indiestudio/scene/PlayScene.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/world/WorldManager.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/ecs/Initializer.hpp"
#include "indiestudio/gameplay/BombFactory.hpp"

namespace IndieStudio {

#define ABS(x) (x > 0 ? x : -x)

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

        // auto &powerup = ecs.addEntity();

        // ecs.setComponent(powerup, Node(Node(static_cast<irr::scene::IAnimatedMeshSceneNode *>(node->clone()))));
        // ecs.setComponent(powerup, MaterialTexture(0, "assets/textures/tmp_powerUp.png"));
        // ecs.setComponent(powerup, MaterialFlag(irr::video::EMF_LIGHTING, true));
        // ecs.setComponent(powerup, Scale(8, 8, 8));
        // ecs.setComponent(powerup, Position(23, 75, 40));
        // ecs.setComponent(powerup, IsPowerUp());
        // ecs.setComponent(powerup, Setup());

        generator->generate(this->pattern.get());

        this->createTerrain();

        for (int i = 0; i < 4; i += 1)
            this->createPlayer(i);
    }

    void World::focusECS(SceneManager::Scene &scene)
    {
        Initializer<WorldECS>::initAllEntities(ecs, scene.scene);

        auto systems = WorldECSSystems(this->ecs, this);

        while (Game::INSTANCE->getSceneManager().getActive() == SceneManager::PLAY_ID
        && Singleton::getDevice()->run()) {
            for (int i = 0; i < 4; i += 1) {
                auto events = this->settings.players[i].controlProviderPtr->pollEvents();

                while (!events.empty()) {
                    this->ecs.getEventManager().push_event(events.front());
                    events.pop_front();
                }
            }

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
                        move({0, 0, 1}, pos, speed, node);
                        did = true;
                    } if (mov.down) {
                        move({0, 0, -1}, pos, speed, node);
                        did = true;
                    } if (mov.left) {
                        move({-1, 0, 0}, pos, speed, node);
                        did = true;
                    } if (mov.right) {
                        move({1, 0, 0}, pos, speed, node);
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
            this->ecs.forEntitiesWith<Stat>([&](auto &data, std::size_t id) {
                (void) id;
                const Stat &stats = this->ecs.getComponent<Stat>(data);
                PlayScene::updateStats(this->scene, stats.playerIdx, stats);
            });
        }
    }

    void World::forwardEvent(ECS::Event::EventData event)
    {
        this->ecs.getEventManager().push_event(event);
    }

    bool World::createBlast(const irr::core::vector3df &position, unsigned int timeMs)
    {
        bool ret = true;
        this->ecs.forEntitiesWith<TextureArray>([&] (auto &data, auto) {
            irr::scene::ISceneNode *sceneNode = this->ecs.getComponent<Node>(data).node;
            irr::core::array<irr::video::ITexture *> &textureArray = this->ecs.getComponent<TextureArray>(data).textureArray;

            if (!sceneNode) {
                ret = false;
                return;
            }

            irr::scene::ISceneNode *clone = sceneNode->clone();
            if (!clone) {
                ret = false;
                return;
            }

            clone->setPosition(position);
            auto del = clone->getSceneManager()->createDeleteAnimator(timeMs);
            if (!del) {
                ret = false;
                return;
            }

            clone->addAnimator(del);
            auto anim = clone->getSceneManager()->createTextureAnimator(textureArray, timeMs / textureArray.size(), false);
            clone->addAnimator(anim);
            clone->setVisible(true);
        });

        return ret;
    }

    void World::move(const irr::core::vector3df &direction, ECS::Position &pos, ECS::Speed &speed, ECS::Node &node)
    {
        irr::core::vector3df actPos(node.node->getAbsolutePosition());

        if (direction.X == -1)
            node.node->setRotation(irr::core::vector3df(0, 90, 0));
        else if (direction.X == 1)
            node.node->setRotation(irr::core::vector3df(0, 270, 0));
        else if (direction.Z == 1)
            node.node->setRotation(irr::core::vector3df(0, 180, 0));
        else if (direction.Z == -1)
            node.node->setRotation(irr::core::vector3df(0, 0, 0));

        irr::core::vector3df newPos(
            actPos.X + (direction.X * speed.x),
            actPos.Y + (direction.Y * speed.y),
            actPos.Z + (direction.Z * speed.z));

        //TODO: check boundingbox at newPos, if can't move return actPos
        node.node->setPosition(newPos);

        pos.x = newPos.X;
        pos.y = newPos.Y;
        pos.z = newPos.Z;
    }

    void World::eject(irr::scene::ISceneNode *node, irr::core::vector3df &bombPos)
    {
        irr::core::vector3df pos(node->getAbsolutePosition());
        irr::core::vector3df dir(pos.X - bombPos.X, 100, pos.Z - bombPos.Z);

        float height = dir.Y - ((ABS(dir.X) - ABS(dir.Z)));
        if (height > 90)
            dir.Y = 10;
        else
            dir.Y -= height;
        dir *= 10;
        auto anim = this->scene.scene->createFlyStraightAnimator(pos, dir, 2000);
        node->addAnimator(anim);
        anim->drop();
    }

    void World::dropBomb(float bombPosX, float bombPosZ, std::size_t playerID)
    {
        IndieStudio::BombFactory::dropBomb<WorldECS>(
            this->ecs, this, this->scene.scene, bombPosX, bombPosZ, playerID);
    }

    void World::pack(ByteBuffer &buffer)
    {
        this->settings.pack(buffer);
        this->pattern->pack(buffer);

        this->ecs.forEntitiesWith<Position, Stat>([&](auto &entity, std::size_t id) {
            (void) entity;

            auto &position = this->ecs.template getComponent<Position>(id);
            auto &stat = this->ecs.template getComponent<Stat>(id);

            buffer << stat.playerIdx;
            buffer << stat.kill;
            buffer << stat.bomb;
            buffer << position.x;
            buffer << position.y;
            buffer << position.z;
        });
    }

    void World::unpack(ByteBuffer &buffer)
    {
        this->settings.unpack(buffer);
        this->pattern = std::make_unique<MapPattern>(this->settings.width,
            this->settings.height);
        this->pattern->unpack(buffer);

        this->createTerrain();

        for (int i = 0; i < 4; i += 1) {
            int playerIdx;
            buffer >> playerIdx;

            auto &player = this->createPlayer(playerIdx);
            auto &position = this->ecs.template getComponent<Position>(player);
            auto &stat = this->ecs.template getComponent<Stat>(player);

            buffer >> stat.kill;
            buffer >> stat.bomb;
            buffer >> position.x;
            buffer >> position.y;
            buffer >> position.z;
        }
    }

    std::size_t World::getBlockEntityIdByPos(short x, short z)
    {
        std::pair<short, short> pos = std::make_pair(x, z);

        for (auto it = this->breakableBlockMapping.begin(); it != this->breakableBlockMapping.end(); ++it)
            if (it->second == pos)
                return it->first;

        throw std::runtime_error("wtf");
    }

    void World::createTerrain()
    {
        auto driver = IndieStudio::Singleton::getDevice()->getVideoDriver();

        auto node = scene.scene->addAnimatedMeshSceneNode(scene.scene->getMesh("assets/models/cube.obj"));
        node->addShadowVolumeSceneNode();
        node->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setScale(irr::core::vector3df(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
		node->setPosition(irr::core::vector3df(0.5, 50, 0.5));
        node->addShadowVolumeSceneNode(node->getMesh());

        this->pattern->forEach([&](int x, int y, int z, MapPattern::TileType tileType) {
	    	if (tileType == MapPattern::TileType::EMPTY)
	    		return;

            auto &newBlock = ecs.addEntity();
            ecs.setComponent(newBlock, Node(static_cast<irr::scene::IAnimatedMeshSceneNode *>(node->clone())));
            auto node = ecs.getComponent<Node>(newBlock).node;
            auto selector = scene.scene->createTriangleSelectorFromBoundingBox(node);
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

            if (y == 1) {
                this->breakableBlockMapping.insert(std::make_pair(newBlock.id,
                    std::make_pair(x, z)));
            }

            //set the powerUp on TileMap
            this->pattern->set(1, 1, 2, MapPattern::TileType::POWER_UP);
        });

        this->initBlast();
    }

    void World::initBlast()
    {
        irr::video::IVideoDriver *driver = Singleton::getDevice()->getVideoDriver();

        auto &test = ecs.addEntity();
        auto node = scene.scene->addAnimatedMeshSceneNode(scene.scene->getMesh("assets/models/cube.obj"));
        node->setVisible(false);

        node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        ecs.setComponent(test, Node(node));
        ecs.setComponent(test, MaterialTexture(0, "assets/textures/bomb.png"));
        ecs.setComponent(test, MaterialFlag(irr::video::EMF_LIGHTING, true));
        ecs.setComponent(test, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
        ecs.setComponent(test, Position(20.5, 90, 100.5));

        irr::core::array<irr::video::ITexture *> textureArray;
        textureArray.push_back(driver->getTexture("assets/textures/flames1.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames2.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames3.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames4.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames5.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames6.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames7.png"));
        textureArray.push_back(driver->getTexture("assets/textures/flames8.png"));
        ecs.setComponent(test, TextureArray(textureArray));
        ecs.setComponent(test, Setup());
    }

    WorldECS::EntityType &World::createPlayer(int playerId)
    {
        auto &player = ecs.addEntity();

        std::pair<short, short> positions[4] = {
            std::make_pair(1 * Constants::TILE_SIZE_FACTOR, (this->settings.height - 2) * Constants::TILE_SIZE_FACTOR),
            std::make_pair(1 * Constants::TILE_SIZE_FACTOR, 1 * Constants::TILE_SIZE_FACTOR),
            std::make_pair((this->settings.width - 2) * Constants::TILE_SIZE_FACTOR, (this->settings.height - 2) * Constants::TILE_SIZE_FACTOR),
            std::make_pair((this->settings.width - 2) * Constants::TILE_SIZE_FACTOR, 1 * Constants::TILE_SIZE_FACTOR)
        };

        auto node_p = scene.scene->addAnimatedMeshSceneNode(scene.scene->getMesh("assets/models/player.md3"));
        node_p->setFrameLoop(0, 27);
        ecs.setComponent(player, Node(node_p));
        ecs.setComponent(player, MaterialTexture(0, "assets/textures/player_" + Constants::PLAYER_COLORS[playerId] + ".png"));
        ecs.setComponent(player, MaterialFlag(irr::video::EMF_LIGHTING, false));
        ecs.setComponent(player, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
        ecs.setComponent(player, Position(positions[playerId].first, 61, positions[playerId].second));
        ecs.setComponent(player, Speed(1, 1, 1));
        ecs.setComponent(player, Movement());
        ecs.setComponent(player, IsPlayer());
        ecs.setComponent(player, MaxBomb(1));

        auto statCmnt = Stat();
        statCmnt.playerIdx = playerId;

        ecs.setComponent(player, statCmnt);
        auto animator = scene.scene->createCollisionResponseAnimator(this->meta, node_p, {5, 5, 5}, {0, 0, 0});
        node_p->addAnimator(animator);
        animator->drop();

        auto eventCB = EventCallbacks<WorldECS>();
        auto &mov = ecs.getComponent<Movement>(player);

        if (this->settings.players[playerId].controlProvider == "AI") {
            ecs.setComponent(player, IA());
        } else {
            eventCB.addCallback(this->settings.players[playerId].mappings.up,
                [&] (const EventData &event, auto, auto) {
                    if (!event.keyInput.PressedDown) {
                        mov.up = event.keyInput.PressedDown;
                        return;
                    }
                    mov.up = event.keyInput.PressedDown;
                    mov.down = false;
                    mov.left = false;
                    mov.right = false;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.down,
                [&] (const EventData &event, auto, auto) {
                    if (!event.keyInput.PressedDown) {
                        mov.down = event.keyInput.PressedDown;
                        return;
                    }
                    mov.down = event.keyInput.PressedDown;
                    mov.up = false;
                    mov.left = false;
                    mov.right = false;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.left,
                [&] (const EventData &event, auto, auto) {
                    if (!event.keyInput.PressedDown) {
                        mov.left = event.keyInput.PressedDown;
                        return;
                    }
                    mov.left = event.keyInput.PressedDown;
                    mov.up = false;
                    mov.down = false;
                    mov.right = false;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.right,
                [&] (const EventData &event, auto, auto) {
                    if (!event.keyInput.PressedDown) {
                        mov.right = event.keyInput.PressedDown;
                        return;
                    }
                    mov.right = event.keyInput.PressedDown;
                    mov.up = false;
                    mov.down = false;
                    mov.left = false;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.drop,
                [&] (auto, auto, auto) {
                    auto &position = this->ecs.template getComponent<Position>(player);
                    auto posInTile = this->pattern->positionToTile(position.x, position.z);

                    this->dropBomb(posInTile.first * 20 + 0.5,
                        posInTile.second * 20 + 0.5, player.id);
                }
            );
        }

        ecs.setComponent(player, eventCB);
        ecs.setComponent(player, Setup());

        return player;
    }

}