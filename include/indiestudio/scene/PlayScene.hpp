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
        ECS::Component::Component1,
        ECS::Component::Component2,
        ECS::Component::Component3
    >;

    using PlayECSSystems = ECS::Systems<
        ECS::System::System1<PlayECS>
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