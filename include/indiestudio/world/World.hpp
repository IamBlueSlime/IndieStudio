/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.hpp
*/

#pragma once

#include <memory>
#include <irrlicht.h>
#include "indiestudio/common/ISerializable.hpp"
#include "indiestudio/ecs/ECSManager.hpp"
#include "indiestudio/scene/SceneManager.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/WorldSettings.hpp"
#include "indiestudio/utils/Packed.hpp"
#include "indiestudio/world/IWorld.hpp"

namespace IndieStudio {

    using WorldECS = ECS::ECSManager<
        ECS::Component::Position,
        ECS::Component::Speed,
        ECS::Component::Alive,
        ECS::Component::Drawable,
        ECS::Component::Movable,
        ECS::Component::Indestructible,
        ECS::Component::LifeTime,
        ECS::Component::ExplosionLifeTime,
        ECS::Component::IsExploding,
        ECS::Component::ExplosionRange,
        ECS::Component::BombType,
        ECS::Component::IsPlayer,
        ECS::Component::IA,
        ECS::Component::IsBomb,
        ECS::Component::IsBlock,
        ECS::Component::IsPowerUp,
        ECS::Component::SoundID,
        ECS::Component::Direction,
        ECS::Component::Scale,
        ECS::Component::MaterialTexture,
        ECS::Component::MaterialFlag,
        ECS::Component::TriangleSelector,
        ECS::Component::MeshPath,
        ECS::Component::NodeCreate,
        ECS::Component::Node,
        ECS::Component::Given,
        ECS::Component::PosedBy,
        ECS::Component::NotSolid,
        ECS::Component::Setup,
        ECS::Component::Movement,
        ECS::Component::Particle,
        ECS::Component::TextureArray,
        ECS::Component::Stat,
        ECS::Component::RealPlayer
    >;

    using WorldECSSystems = ECS::SystemsImpl<
        WorldECS,
        ECS::System::EventSystem<WorldECS>,
        ECS::System::ExplosionDurationSystem<WorldECS>,
        ECS::System::ApplyExplosionSystem<WorldECS>,
        ECS::System::SetupExplosionSystem<WorldECS>,
        ECS::System::PickUpPowerUpSystem<WorldECS>,
        ECS::System::IASystem<WorldECS>,
        ECS::System::SolidBombSystem<WorldECS>,
        ECS::System::WinLooseSystem<WorldECS>,
        ECS::System::MovePlayerSystem<WorldECS>
    >;

    class WorldManager;

    class World : public IWorld, public ISerializable {
    public:
        PACKED(struct FileHeader {
            unsigned char magic[4];
            unsigned char formatVersion;
            unsigned long size;
            unsigned int checksum;
        });

        World(WorldSettings settings);
        World();

        void create(WorldManager &manager);

        void focusECS(SceneManager::Scene &scene);
        void forwardEvent(ECS::Event::EventData event);

        void eject(irr::scene::ISceneNode *node, irr::core::vector3df &bombPos);
        bool createBlast(const irr::core::vector3df &position, unsigned int timeMs = 1000);
        void createPowerUp(std::size_t type, const ECS::Position &position);
        void move(const irr::core::vector3df &direction, ECS::Position &pos, ECS::Speed &speed, ECS::Node &node);
        void dropBomb(float bombPosX, float bombPosZ, std::size_t playerID, float range) override;
        void dropRandomBomb();

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) override;
        void unpack(ByteBuffer &buffer) override;

        std::size_t getBlockEntityIdByPos(short x, short z);

        WorldSettings &getSettings() { return this->settings; }
        MapPattern *getPattern() override { return this->pattern.get(); }
        const WorldSettings &getSettings() const { return this->settings; }
        irr::scene::IMetaTriangleSelector *getMeta() { return this->meta; }
        SceneManager::Scene &getScene() { return this->scene; }

    protected:
    private:
        WorldSettings settings;
        std::unique_ptr<MapPattern> pattern;
        std::unordered_map<std::size_t, std::pair<short, short>> breakableBlockMapping;
        WorldECS ecs;
        SceneManager::Scene &scene;
        irr::scene::IMetaTriangleSelector *meta;

        void createTerrain();
        void initBlast();
        WorldECS::EntityType &createPlayer(int playerId);
    };

}
