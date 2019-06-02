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
        static void setupMenu(SceneManager::Scene &scene);
        static bool onEvent(SceneManager::Scene &scene, const irr::SEvent &event);

        enum {
            BUTTON_ID_BACK,
            BUTTON_ID_CONFIRM
        };
    };

}