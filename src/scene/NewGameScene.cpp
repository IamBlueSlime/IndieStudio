/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene NewGameScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/NewGameScene.hpp"
#include "indiestudio/scene/PlayScene.hpp"

namespace IndieStudio {

    void NewGameScene::initialize(SceneManager::Scene &scene)
    {
        scene.scene->addSkyDomeSceneNode(scene.manager->textureManager.getTexture("assets/textures/skydome.jpg").content);

        scene.scene->addCameraSceneNode(0, irr::core::vector3df(50, 0, 0), irr::core::vector3df(0, 0, 0));

        setupMenu(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };
    }

    void NewGameScene::setupMenu(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        guiEnv->addImage(
            scene.manager->textureManager.getTexture("assets/textures/title.png").content, irr::core::position2di(350, 75), true, guiRoot);
        
        guiEnv->addStaticText(L"Map size:",
            irr::core::recti({500, 280}, {750, 300}), false, true, guiRoot);
        irr::gui::IGUIScrollBar *sizeBar = guiEnv->addScrollBar(true,
            irr::core::recti({500, 300}, {750, 320}), guiRoot, 32);
        sizeBar->setMin(1);
        sizeBar->setMax(3);
        sizeBar->setSmallStep(1);
        sizeBar->setLargeStep(1);

        irr::core::vector2di pos(395, 610);
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_BACK, L"Back");
        pos.X += 240;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_CONFIRM, L"Confirm");
    }

    bool NewGameScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        (void) scene;
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
                return true;
            }
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller->getID() == BUTTON_ID_BACK) {
                scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
                return true;
            } else if (event.GUIEvent.Caller->getID() == BUTTON_ID_CONFIRM) {
                WorldManager &manager = static_cast<WorldManager &>(
                    Game::INSTANCE->getWorldManager());

                WorldSettings settings;
                int mapScale = static_cast<irr::gui::IGUIScrollBar *>(
                    scene.gui->getElementFromId(32)
                )->getPos();
                settings.width *= mapScale;
                settings.height *= mapScale;

                World *world = manager.create(settings);
                SceneManager::Scene &playScene = scene.manager->getScene(SceneManager::PLAY_ID);
                PlayScene::initialize(playScene);
                scene.manager->setActiveScene(SceneManager::PLAY_ID);
                world->focusECS(playScene.scene);

                return true;
            }
        }

        return false;
    }

}