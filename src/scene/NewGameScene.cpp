/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene NewGameScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/MainMenuScene.hpp"
#include "indiestudio/scene/NewGameScene.hpp"
#include "indiestudio/scene/PlayScene.hpp"

namespace IndieStudio {

    WorldSettings NewGameScene::settings = WorldSettings();

    void NewGameScene::initialize(SceneManager::Scene &scene)
    {
        settings = WorldSettings();

        scene.scene->addSkyDomeSceneNode(scene.manager->textureManager.getTexture("assets/textures/skydome.jpg").content);
        scene.scene->addCameraSceneNode(0, irr::core::vector3df(0, 0, -50), irr::core::vector3df(0, 0, 0));

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
            scene.manager->textureManager.getTexture("assets/textures/title.png").content,
            irr::core::position2di(300, 75), true, guiRoot
        );

        irr::core::vector2di origin(410, 250);

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

        origin.X -= 240;
        origin.Y += 20;
        for (int i = 0; i < 4; i += 1) {
            irr::core::vector2di playerOrigin = {origin.X + (240 * i), origin.Y};
            setupPlayer(scene, playerOrigin, i);
        }
        updateKeyboardButtons(scene);

        origin.X += 240;
        irr::core::vector2di pos(origin.X, 610);
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_BACK, L"Back");
        pos.X += 240;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_CONFIRM, L"Confirm");
    }

    void NewGameScene::setupPlayer(SceneManager::Scene &scene, irr::core::vector2di origin,
        int idx)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        irr::scene::IAnimatedMeshSceneNode* anms = scene.scene->addAnimatedMeshSceneNode(
            scene.scene->getMesh("assets/models/player.md3"), 0);

        if (!anms)
            throw std::runtime_error("Failed to load player.md3");

        anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        anms->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_" + Constants::PLAYER_COLORS[idx] + ".png").content);
        anms->setFrameLoop(0, 27);
        anms->setAnimationSpeed(30);
        anms->setScale(irr::core::vector3df(8.f,8.f,8.f));
        anms->setAnimationSpeed(18);

        float deltas[4] = {-35, -10, 10, 35};
        anms->setPosition(irr::core::vector3df(deltas[idx], -10, 0));

        origin.Y += 200;

        irr::gui::IGUIComboBox *playerTypeBox = guiEnv->addComboBox(irr::core::recti(
            {origin.X, origin.Y},
            {origin.X + 225, origin.Y + 20}
        ), guiRoot, 42 + (idx * 10));

        playerTypeBox->addItem(L"Keyboard");
        playerTypeBox->addItem(L"Controller");
        playerTypeBox->addItem(L"AI");
        playerTypeBox->setSelected(static_cast<irr::s32>(settings.players[idx].controlType));

        origin.X += 20;
        origin.Y += 40;
        guiEnv->addButton(irr::core::recti(
            {origin.X + 35, origin.Y}, {origin.X + 35 + 30, origin.Y + 30}
        ), guiRoot, 42 + (idx * 10) + 1, L"?", L"Up key");

        guiEnv->addButton(irr::core::recti(
            {origin.X, origin.Y + 35}, {origin.X + 30, origin.Y + 35 + 30}
        ), guiRoot, 42 + (idx * 10) + 2, L"?", L"Left key");

        guiEnv->addButton(irr::core::recti(
            {origin.X + 35, origin.Y + 35}, {origin.X + 35 + 30, origin.Y + 35 + 30}
        ), guiRoot, 42 + (idx * 10) + 3, L"?", L"Down key");

        guiEnv->addButton(irr::core::recti(
            {origin.X + 35 + 35, origin.Y + 35}, {origin.X + 35 + 35 + 30, origin.Y + 35 + 30}
        ), guiRoot, 42 + (idx * 10) + 4, L"?", L"Right key");

        guiEnv->addButton(irr::core::recti(
            {origin.X + 150, origin.Y + 17}, {origin.X + 150 + 30, origin.Y + 17 + 30}
        ), guiRoot, 42 + (idx * 10) + 5, L"?", L"Drop key");
    }

    void NewGameScene::updateKeyboardButtons(SceneManager::Scene &scene)
    {
        for (int i = 0; i < 4; i += 1) {
            irr::gui::IGUIComboBox *comboBox = static_cast<irr::gui::IGUIComboBox *>(
                scene.gui->getElementFromId(42 + (i * 10)));

            bool visible = comboBox->getSelected() == 0;

            irr::EKEY_CODE *codes[5] = {
                &settings.players[i].keyboardUp,
                &settings.players[i].keyboardLeft,
                &settings.players[i].keyboardDown,
                &settings.players[i].keyboardRight,
                &settings.players[i].keyboardDrop
            };

            for (int j = 1; j < 6; j += 1) {
                std::wstring tmp;
                tmp += *codes[j - 1];
                scene.gui->getElementFromId(42 + (i * 10) + j)->setText(tmp.c_str());
                scene.gui->getElementFromId(42 + (i * 10) + j)->setVisible(visible);
            }
        }
    }

    bool NewGameScene::isValidKeyboardKey(irr::EKEY_CODE code)
    {
        if (code < irr::KEY_KEY_0 || code > irr::KEY_KEY_Z)
            return false;

        for (int i = 0; i < 4; i += 1) {
            irr::EKEY_CODE *codes[5] = {
                &settings.players[i].keyboardUp,
                &settings.players[i].keyboardLeft,
                &settings.players[i].keyboardDown,
                &settings.players[i].keyboardRight,
                &settings.players[i].keyboardDrop
            };

            for (int j = 1; j < 6; j += 1) {
                if (*codes[j - 1] == code)
                    return false;
            }
        }

        return true;
    }

    bool NewGameScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        (void) scene;
        if (event.EventType == irr::EET_KEY_INPUT_EVENT
        && event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
            scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
            return true;
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller->getID() == BUTTON_ID_BACK) {
                scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
                return true;
            } else if (event.GUIEvent.Caller->getID() == BUTTON_ID_CONFIRM) {
                MainMenuScene::BACKGROUND_MUSIC.stop();

                WorldManager &manager = static_cast<WorldManager &>(
                    Game::INSTANCE->getWorldManager());

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
                world->focusECS(playScene);

                return true;
            } else if (event.GUIEvent.Caller->getID() > 42) {
                irr::gui::IGUIButton *button = static_cast<irr::gui::IGUIButton *>(
                    event.GUIEvent.Caller);

                if (button->isPressed()) {
                    button->setPressed(false);
                    updateKeyboardButtons(scene);
                } else {
                    button->setText(L"?");
                    button->setPressed(true);
                }

                return true;
            }
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_COMBO_BOX_CHANGED) {
            irr::gui::IGUIComboBox *comboBox = static_cast<irr::gui::IGUIComboBox *>(
                event.GUIEvent.Caller);

            if (comboBox->getID() < 42)
                return false;

            if (comboBox->getSelected() != -1)
                settings.players[(comboBox->getID() - 42) / 10].controlType = static_cast<WorldSettings::Player::ControlType>(comboBox->getSelected());
            updateKeyboardButtons(scene);
            return true;
        } else if (event.EventType == irr::EET_KEY_INPUT_EVENT
        && isValidKeyboardKey(event.KeyInput.Key)
        && event.KeyInput.PressedDown) {
            for (int i = 0; i < 4; i += 1) {
                irr::EKEY_CODE *codes[5] = {
                    &settings.players[i].keyboardUp,
                    &settings.players[i].keyboardLeft,
                    &settings.players[i].keyboardDown,
                    &settings.players[i].keyboardRight,
                    &settings.players[i].keyboardDrop
                };

                for (int j = 1; j < 6; j += 1) {
                    irr::gui::IGUIButton *button = static_cast<irr::gui::IGUIButton *>(
                        scene.gui->getElementFromId(42 + (i * 10) + j));

                    if (button->isPressed()) {
                        *codes[j - 1] = event.KeyInput.Key;
                        button->setPressed(false);
                        updateKeyboardButtons(scene);
                        return true;
                    }
                }
            }

            return true;
        }

        return false;
    }

}