/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene PlayScene.hpp
*/

#pragma once

#include "indiestudio/ecs/ECSManager.hpp"
#include "indiestudio/scene/SceneManager.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio {

    using PlayECS = ECS::ECSManager<
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
        ECS::Component::IsAI,
        ECS::Component::IsBomb,
        ECS::Component::SoundID,
        ECS::Component::Direction,
        ECS::Component::Scale,
        ECS::Component::MaterialTexture,
        ECS::Component::MaterialFlag,
        ECS::Component::MeshPath,
        ECS::Component::NodeCreate,
        ECS::Component::Node,
        ECS::Component::ID
        // ECS::Component::EventCallbacks<PlayECS>
    >;

    using PlayECSSystems = ECS::Systems<
        ECS::System::ApplyExplosion<PlayECS>,
        ECS::System::EventSystem<PlayECS>,
        ECS::System::ExplosionDuration<PlayECS>,
        ECS::System::MovePlayer<PlayECS>,
        ECS::System::SetupExplosion<PlayECS>
    >;

    class PlayScene {
	public:
		static void initialize(SceneManager::Scene &scene);
        static void createWorldFromSettings(SceneManager::Scene &scene,
            const WorldSettings &settings, const MapPattern &pattern);
        static void createWorldFromDump(SceneManager::Scene &scene, World *world);

	protected:
	private:
        static const int SQUARED_SIZE;
        static const float FLOOR_Y;
        static PlayECS ECS_INSTANCE;

        static void setupWaterBackground(SceneManager::Scene &scene);
        static void setupLight(SceneManager::Scene &scene);
        static void setupTravelling(SceneManager::Scene &scene);

        static void createPlayer(SceneManager::Scene &scene, irr::core::vector3df position);

        static bool onEvent(SceneManager::Scene &scene, const irr::SEvent &event);
    };

}