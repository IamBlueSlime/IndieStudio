/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene NewGameScene.hpp
*/

#pragma once

#include "indiestudio/scene/SceneManager.hpp"

namespace IndieStudio {

    class NewGameScene {
    public:
        static void initialize(SceneManager::Scene &scene);
    
    protected:
    private:
        static WorldSettings settings;

        static void setupMenu(SceneManager::Scene &scene);
        static void setupPlayer(SceneManager::Scene &scene, irr::core::vector2di origin,
            int idx);

        static void updateKeyboardButtons(SceneManager::Scene &scene);
        static bool isValidKeyboardKey(irr::EKEY_CODE code);

        static bool onEvent(SceneManager::Scene &scene, const irr::SEvent &event);

        enum {
            BUTTON_ID_BACK,
            BUTTON_ID_CONFIRM
        };
    };

}