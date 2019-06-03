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
        
        irr::core::vector2di origin(395, 250);

        guiEnv->addStaticText(L"World size:", irr::core::recti(
            {origin.X, origin.Y},
            {origin.X + 225, origin.Y + 20}
        ), false, true, guiRoot);
        guiEnv->addStaticText(L"World generator:", irr::core::recti(
            {origin.X + 240, origin.Y},
            {origin.X + 240 + 225, origin.Y + 20}
        ), false, true, guiRoot);
        origin.Y += 20;

        irr::gui::IGUIScrollBar *sizeBar = guiEnv->addScrollBar(true, irr::core::recti(
            {origin.X, origin.Y},
            {origin.X + 225, origin.Y + 20}
        ), guiRoot, 32);
        sizeBar->setMin(1);
        sizeBar->setMax(3);
        sizeBar->setSmallStep(1);
        sizeBar->setLargeStep(1);

        irr::gui::IGUIComboBox *generatorBox = guiEnv->addComboBox(irr::core::recti(
            {origin.X + 240, origin.Y},
            {origin.X + 240 + 225, origin.Y + 20}
        ), guiRoot, 33);

        WorldManager &worldManager = static_cast<WorldManager &>(Game::INSTANCE->getWorldManager());
        int i = 0;
        for (auto it = worldManager.getGenerators().begin(); it != worldManager.getGenerators().end(); ++it) {
            generatorBox->addItem(std::wstring(it->first.begin(), it->first.end()).c_str(), i);
            i += 1;
        }

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

                irr::gui::IGUIComboBox *generatorBox = static_cast<irr::gui::IGUIComboBox *>(
                    scene.gui->getElementFromId(33));
                std::wstring generatorName = std::wstring(generatorBox->getItem(generatorBox->getSelected()));
                settings.worldGenerator = std::string(generatorName.begin(), generatorName.end());

                SceneManager::Scene &playScene = scene.manager->getScene(SceneManager::PLAY_ID);
                playScene.scene->clear();
                World *world = manager.create(settings);
                PlayScene::initialize(playScene);
                scene.manager->setActiveScene(SceneManager::PLAY_ID);
                world->focusECS(playScene.scene);

                return true;
            }
        }

        return false;
    }

}