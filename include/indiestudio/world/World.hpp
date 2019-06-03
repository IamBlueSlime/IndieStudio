/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.hpp
*/

#pragma once

#include <irrlicht/irrlicht.h>
#include "indiestudio/common/ISerializable.hpp"
#include "indiestudio/ecs/ECSManager.hpp"
#include "indiestudio/scene/SceneManager.hpp"
#include "indiestudio/world/WorldSettings.hpp"

namespace IndieStudio {

    using WorldECS = ECS::ECSManager<
        // ECS::Component::Position,
        // ECS::Component::Speed,
        // ECS::Component::Alive,
        // ECS::Component::Drawable,
        // ECS::Component::Movable,
        // ECS::Component::Indestructible,
        // ECS::Component::LifeTime,
        // ECS::Component::ExplosionLifeTime,
        // ECS::Component::IsExploding,
        // ECS::Component::ExplosionRange,
        // ECS::Component::BombType,
        // ECS::Component::IsPlayer,
        // ECS::Component::IsAI,
        // ECS::Component::IsBomb,
        // ECS::Component::SoundID,
        // ECS::Component::Direction,
        // ECS::Component::Scale,
        // ECS::Component::MaterialTexture,
        // ECS::Component::MaterialFlag,
        // ECS::Component::MeshPath,
        // ECS::Component::NodeCreate,
        // ECS::Component::Node,
        // ECS::Component::ID
        // ECS::Component::EventCallbacks<WorldECS>
    >;

    using WorldECSSystems = ECS::SystemsImpl<
        WorldECS
        // ECS::System::ApplyExplosion<WorldECS>,
        // ECS::System::EventSystem<WorldECS>,
        // ECS::System::ExplosionDuration<WorldECS>,
        // ECS::System::MovePlayer<WorldECS>,
        // ECS::System::SetupExplosion<WorldECS>
    >;

    class WorldManager;

    class World : public ISerializable {
    public:
        struct FileHeader {
            unsigned char magic[4];
            unsigned char formatVersion;
            unsigned long size;
            unsigned int checksum;
        } __attribute__((packed));

        World(WorldSettings settings);
        World();

        void create(WorldManager &manager);

        void focusECS(irr::scene::ISceneManager *sceneManager);
        void forwardEvent(ECS::Event::EventData &event);

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override;
        void unpack(ByteBuffer &buffer) override;

        WorldSettings &getSettings() { return this->settings; }
        const WorldSettings &getSettings() const { return this->settings; }

    protected:
    private:
        WorldSettings settings;
        WorldECS ecs;
        SceneManager::Scene &scene;
    };

}