/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/common/Error.hpp"
#include "indiestudio/Constants.hpp"
#include "indiestudio/common/Scheduler.hpp"
#include "indiestudio/ecs/Events.hpp"
#include "indiestudio/scene/PlayScene.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/world/WorldManager.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/ecs/Initializer.hpp"
#include "indiestudio/gameplay/BombFactory.hpp"
#include "indiestudio/common/Error.hpp"
#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    World::World(WorldSettings settings)
        : settings(settings),
        pattern(std::make_unique<MapPattern>(settings.width, settings.height)),
        scene(Game::INSTANCE->getSceneManager().getScene(SceneManager::PLAY_ID)),
        meta(this->scene.scene->createMetaTriangleSelector())
    {}

    World::World() : World(WorldSettings())
    {}

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
            throw IndieError(
                "Failed to find the world generator " + this->settings.worldGenerator);

        generator->generate(this->pattern.get());

        // Because we fill the empty space randomly, we ensure that
        // the spawn areas are usable.
        this->pattern->set(1, 1, 1, MapPattern::TileType::EMPTY);
        this->pattern->set(1, 1, 2, MapPattern::TileType::EMPTY);
        this->pattern->set(2, 1, 1, MapPattern::TileType::EMPTY);
        this->pattern->set(this->pattern->getWidth() - 2, 1, 1, MapPattern::TileType::EMPTY);
        this->pattern->set(this->pattern->getWidth() - 2, 1, 2, MapPattern::TileType::EMPTY);
        this->pattern->set(this->pattern->getWidth() - 3, 1, 1, MapPattern::TileType::EMPTY);
        this->pattern->set(1, 1, this->pattern->getHeight() - 2, MapPattern::TileType::EMPTY);
        this->pattern->set(1, 1, this->pattern->getHeight() - 3, MapPattern::TileType::EMPTY);
        this->pattern->set(2, 1, this->pattern->getHeight() - 2, MapPattern::TileType::EMPTY);
        this->pattern->set(this->pattern->getWidth() - 2, 1, this->pattern->getHeight() - 2, MapPattern::TileType::EMPTY);
        this->pattern->set(this->pattern->getWidth() - 2, 1, this->pattern->getHeight() - 3, MapPattern::TileType::EMPTY);
        this->pattern->set(this->pattern->getWidth() - 3, 1, this->pattern->getHeight() - 2, MapPattern::TileType::EMPTY);

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
            // Ultimate security system \o/
            if (Game::INSTANCE->getSceneManager().getActive() != SceneManager::PLAY_ID)
                break;

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
            this->ecs.getEventManager().clear_event_queue();
            scene.scene->drawAll();
            scene.gui->draw();
            scene.scene->getVideoDriver()->endScene();
            Scheduler::tick();
            this->ecs.forEntitiesWith<Stat>([&](auto &data, auto) {
                const Stat &stats = this->ecs.getComponent<Stat>(data);
                PlayScene::updateStats(this->scene, stats.playerIdx, stats);
            });
        }

        irr::core::list<irr::gui::IGUIElement *> children = scene.gui->getChildren();

        for (irr::gui::IGUIElement *element : children)
            scene.gui->removeChild(element);

        this->meta->drop();
        scene.scene->getRootSceneNode()->removeAll();
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
            auto del = clone->getSceneManager()->createDeleteAnimator(timeMs + 300);
            if (!del) {
                ret = false;
                return;
            }

            clone->addAnimator(del);
            auto anim = clone->getSceneManager()->createTextureAnimator(textureArray, (timeMs + 300) / textureArray.size(), false);
            clone->addAnimator(anim);
            clone->setVisible(true);
        });

        return ret;
    }

    void World::createPowerUp(std::size_t type, const Position &pos) {
        auto node = scene.scene->addBillboardSceneNode(nullptr, irr::core::dimension2df(
            Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR),
            irr::core::vector3df(pos.x, 73, pos.z)
        );
        auto &powerup = ecs.addEntity();
        std::pair<short, short> coord = MapPattern::positionToTile(pos.x, pos.z);
        this->pattern->set(coord.first, 1, coord.second, MapPattern::TileType::POWER_UP);
        ecs.setComponent(powerup, Node(node));
        ecs.setComponent(powerup, MaterialFlag(irr::video::EMF_LIGHTING, true));
        ecs.setComponent(powerup, Scale(5, 5, 5));
        ecs.setComponent(powerup, Position(pos.x, 73, pos.z));
        ecs.setComponent(powerup, IsPowerUp(type));
        ecs.setComponent(powerup, Setup());

        if (type == IsPowerUp::SPEED_POWERUP)
            ecs.setComponent(powerup, MaterialTexture(0, "assets/textures/powerups/speed_up.png"));
        else if (type == IsPowerUp::STOCK_POWERUP)
            ecs.setComponent(powerup, MaterialTexture(0, "assets/textures/powerups/bomb_up.png"));
        else
            ecs.setComponent(powerup, MaterialTexture(0, "assets/textures/powerups/bomb_range_up.png"));

        IndieStudio::Initializer<WorldECS>::initAllEntities(this->ecs, this->scene.scene);
        irr::scene::ISceneManager *scenemg = node->getSceneManager();
        irr::core::vector3df newPos = node->getPosition();
        newPos.Y += 3;
        auto anim = scenemg->createFlyStraightAnimator(node->getPosition(), newPos, 1500, true, true);
        node->addAnimator(anim);
        anim->drop();
    }

    void World::move(const irr::core::vector3df &direction, ECS::Position &pos, ECS::Speed &speed, ECS::Node &node)
    {
        irr::core::vector3df actPos(pos.x, pos.y, pos.z);
        irr::core::vector3df rota(0, 0, 0);

        if (direction.X == -1) {
            rota.Y = 90;
            if (direction.Z != 0)
                rota.Y += (direction.Z == 1) ? 45 : -45;
            node.node->setRotation(rota);
        } else if (direction.X == 1) {
            rota.Y = 270;
            if (direction.Z != 0)
                rota.Y += (direction.Z == 1) ? -45 : 45;
            node.node->setRotation(rota);
        } else if (direction.Z == 1) {
            rota.Y = 180;
            if (direction.X != 0)
                rota.Y += (direction.X == 1) ? 45 : -45;
            node.node->setRotation(rota);
        } else if (direction.Z == -1) {
            rota.Y = 0;
            if (direction.X != 0)
                rota.Y += (direction.X == 1) ? -45 : 45;
            node.node->setRotation(rota);
        }

        irr::core::vector3df newPos(
            actPos.X + (direction.X * speed.x),
            actPos.Y + (direction.Y * speed.y),
            actPos.Z + (direction.Z * speed.z));

        node.node->setPosition(newPos);
    }

    void World::eject(irr::scene::ISceneNode *node, irr::core::vector3df &bombPos)
    {
        irr::core::vector3df pos(node->getAbsolutePosition());
        irr::core::vector3df dir(pos.X - bombPos.X, 100, pos.Z - bombPos.Z);

        float height = dir.Y - std::abs(dir.X) - std::abs(dir.Z);
        if (height > 90)
            dir.Y = 10;
        else
            dir.Y -= height;
        dir *= 20;
        auto anim = this->scene.scene->createRotationAnimator(irr::core::vector3df(0, 50, 0));
        node->addAnimator(anim);
        anim->drop();
        anim = this->scene.scene->createFlyStraightAnimator(pos, dir, 1000);
        node->addAnimator(anim);
        anim->drop();
    }

    void World::dropBomb(float bombPosX, float bombPosZ, std::size_t playerID, float range)
    {
        IndieStudio::BombFactory::dropBomb<WorldECS>(
            this->ecs, this, this->scene.scene, bombPosX, bombPosZ, playerID, range);
    }

    void World::dropRandomBomb()
    {
        unsigned short realWidth = this->settings.width - 2;
        unsigned short realHeight = this->settings.height - 2;
        float posX = 1 + std::rand() % realWidth;
        float posZ = 1 + std::rand() % realHeight;

        bool done = false;

        this->ecs.forEntitiesWith<ECS::Component::Stat, ECS::Component::Alive>([&](auto, std::size_t id) {
            ECS::Component::Stat &stats = this->ecs.getComponent<ECS::Component::Stat>(id);
            done = true;
            this->dropBomb(posX, posZ, id, stats.range);
        });
    }

    void World::pack(ByteBuffer &buffer)
    {
        this->settings.pack(buffer);

        for (int i = 0; i < 4; i += 1) {
            ByteBufferUtils::writeString(buffer, this->settings.players[i].controlProvider);
            this->settings.players[i].controlProviderPtr->pack(buffer, i);
        }

        this->pattern->pack(buffer);

        this->ecs.forEntitiesWith<Position, Stat>([&](auto &entity, std::size_t id) {
            (void) entity;

            auto &position = this->ecs.template getComponent<Position>(id);
            auto &stat = this->ecs.template getComponent<Stat>(id);

            buffer << stat.playerIdx;
            buffer << stat.kill;
            buffer << stat.bomb;
            buffer << stat.range;
            buffer << position.x;
            buffer << position.y;
            buffer << position.z;
        });
    }

    void World::unpack(ByteBuffer &buffer)
    {
        this->settings.unpack(buffer);

        for (int i = 0; i < 4; i += 1) {
            std::string controlProvider = ByteBufferUtils::readString(buffer);

            try {
                auto controlProviderPtr = Game::INSTANCE->getControlProviderManager().getControlProvider(controlProvider);
                this->settings.players[i].controlProvider = controlProvider;
                this->settings.players[i].controlProviderPtr = controlProviderPtr;
                this->settings.players[i].controlProviderPtr->unpack(buffer, i);
                this->settings.players[i].mappings = this->settings.players[i].controlProviderPtr->getPlayerMappings(i);
            } catch (const ProviderError &ex) {
                throw SaveError("Failed to find the control provider '" + controlProvider + "'");
            }
        }

        this->pattern = std::make_unique<MapPattern>(this->settings.width,
            this->settings.height);
        this->pattern->clear();
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
            buffer >> stat.range;
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

        throw IndieError("wtf");
    }

    void World::createTerrain()
    {
        auto driver = IndieStudio::Singleton::getDevice()->getVideoDriver();

        auto node = scene.scene->addAnimatedMeshSceneNode(scene.scene->getMesh("assets/models/cube.obj"));
        // node->addShadowVolumeSceneNode();
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
            auto copyNode = ecs.getComponent<Node>(newBlock).node;
            auto selector = scene.scene->createTriangleSelectorFromBoundingBox(copyNode);
            copyNode->setTriangleSelector(selector);
            this->meta->addTriangleSelector(selector);
            selector->drop();

            ecs.setComponent(newBlock, Position(
                copyNode->getPosition().X + copyNode->getScale().X * x,
                copyNode->getPosition().Y + copyNode->getScale().Y * (y == 1 ? 1 : 0),
                copyNode->getPosition().Z + copyNode->getScale().Z * z
            ));

            auto animatedNode = static_cast<irr::scene::IAnimatedMeshSceneNode *>
                (ecs.getComponent<Node>(newBlock).node);

                if (tileType == MapPattern::TileType::FLOOR_FIRST) {
                animatedNode->addShadowVolumeSceneNode();
                        ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_1.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
                } else if (tileType == MapPattern::TileType::FLOOR_SECOND) {
                animatedNode->addShadowVolumeSceneNode();
                    ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_ground_2.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
                } else if (tileType == MapPattern::TileType::BORDER_WALL_BLOCK) {
                animatedNode->addShadowVolumeSceneNode();
                        ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
                } else if (tileType == MapPattern::TileType::INNER_WALL_BLOCK) {
                animatedNode->addShadowVolumeSceneNode();
                        ecs.setComponent(newBlock, MaterialTexture(0, "assets/textures/block_wall.png"));
                ecs.setComponent(newBlock, Scale(Constants::TILE_SIZE_FACTOR * 0.9, Constants::TILE_SIZE_FACTOR * 0.9, Constants::TILE_SIZE_FACTOR * 0.9));
            } else if (tileType == MapPattern::TileType::BREAKABLE_BLOCK) {
                animatedNode->addShadowVolumeSceneNode();
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
        node_p->setFrameLoop(27, 76);
        ecs.setComponent(player, Node(node_p));
        ecs.setComponent(player, MaterialTexture(0, "assets/textures/player_" + Constants::PLAYER_COLORS[playerId] + ".png"));
        ecs.setComponent(player, MaterialFlag(irr::video::EMF_LIGHTING, false));
        ecs.setComponent(player, Scale(Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR, Constants::TILE_SIZE_FACTOR));
        ecs.setComponent(player, Position(positions[playerId].first, 61, positions[playerId].second));
        ecs.setComponent(player, Speed(1, 1, 1));
        ecs.setComponent(player, Movement());
        ecs.setComponent(player, IsPlayer());
        ecs.setComponent(player, Alive());

        auto statCmnt = Stat();
        statCmnt.playerIdx = playerId;
        statCmnt.bomb = 1;
        statCmnt.range = 2;
        statCmnt.alive = true;
        statCmnt.draw = false;
        statCmnt.winner = false;

        ecs.setComponent(player, statCmnt);
        auto animator = scene.scene->createCollisionResponseAnimator(this->meta, node_p, {5, 5, 5}, {0, 0, 0});
        node_p->addAnimator(animator);
        animator->drop();

        auto eventCB = EventCallbacks<WorldECS>();
        auto &mov = ecs.getComponent<Movement>(player);

        if (this->settings.players[playerId].controlProvider == "AI") {
            ecs.setComponent(player, IA());
        } else {
            ecs.setComponent(player, RealPlayer());
            eventCB.addCallback(this->settings.players[playerId].mappings.up,
                [&] (const EventData &event, auto, auto) {
                    mov.up = event.keyInput.PressedDown;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.down,
                [&] (const EventData &event, auto, auto) {
                    mov.down = event.keyInput.PressedDown;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.left,
                [&] (const EventData &event, auto, auto) {
                    mov.left = event.keyInput.PressedDown;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.right,
                [&] (const EventData &event, auto, auto) {
                    mov.right = event.keyInput.PressedDown;
                }
            );
            eventCB.addCallback(this->settings.players[playerId].mappings.drop,
                [&] (auto, auto, auto) {
                    try {
                        ecs.template getComponent<Alive>(player);
                    } catch (...) {
                        return;
                    }
                    auto &position = this->ecs.template getComponent<Position>(player);
                    auto stat = this->ecs.template getComponent<Stat>(player);
                    auto posInTile = this->pattern->positionToTile(position.x, position.z);

                    this->dropBomb(posInTile.first * 20 + 0.5,
                        posInTile.second * 20 + 0.5, player.id, stat.range);
                }
            );
        }

        ecs.setComponent(player, eventCB);
        ecs.setComponent(player, Setup());

        return player;
    }

}
