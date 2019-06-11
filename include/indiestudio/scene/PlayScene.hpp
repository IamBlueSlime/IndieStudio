/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene PlayScene.hpp
*/

#pragma once

#include <SFML/Audio.hpp>
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/scene/SceneManager.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio {

    class PlayScene {
	public:
		static void initialize(SceneManager::Scene &scene);
        static void updateStats(SceneManager::Scene &scene, int playerIdx,
            const ECS::Component::Stat &stats);

	protected:
	private:
        static const float FLOOR_Y;
        static sf::Sound COUNTDOWN_SOUND;
        static sf::Sound BACKGROUND_MUSIC;
        static std::size_t TIMER_TASK;

        static void setupLight(SceneManager::Scene &scene);
        static void setupTravelling(SceneManager::Scene &scene);
        static void setupOverlay(SceneManager::Scene &scene);
        static void setupCountdown(SceneManager::Scene &scene);

        static void updateTimer(SceneManager::Scene &scene);

        static bool onEvent(SceneManager::Scene &scene, const irr::SEvent &event);
    };

}