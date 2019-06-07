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
    std::unordered_map<std::string, irr::gui::IGUITab *> NewGameScene::areas[4];

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

        origin.Y += 190;

        irr::gui::IGUIComboBox *playerTypeBox = guiEnv->addComboBox(irr::core::recti(
            {origin.X, origin.Y},
            {origin.X + 225, origin.Y + 20}
        ), guiRoot, 42 + (idx * 10));

        Registry<std::string, IControlProvider *> &controlProviders =
            static_cast<ControlProviderManager &>(Game::INSTANCE->getControlProviderManager()).getControlProviders();

        controlProviders.forEach([&](std::string id, IControlProvider *controlProvider) {
            int itemIdx = playerTypeBox->addItem(std::wstring(id.begin(), id.end()).c_str());

            if (settings.players[idx].controlProvider == id)
                playerTypeBox->setSelected(itemIdx);

            irr::gui::IGUITab *providerTab = guiEnv->addTab(irr::core::recti(
                {origin.X, origin.Y + 40},
                {origin.X + 225, origin.Y + 40 + 80}
            ), guiRoot);

            providerTab->setVisible(false);
            controlProvider->initConfigurationArea(guiEnv, providerTab, idx);
            areas[idx].insert(std::make_pair(id, providerTab));
        });

        areas[idx][settings.players[idx].controlProvider]->setVisible(true);
    }

    bool NewGameScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT
        && event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
            scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
            return true;
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED
        && event.GUIEvent.Caller->getID() == BUTTON_ID_BACK) {
            scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
            return true;
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED
        && event.GUIEvent.Caller->getID() == BUTTON_ID_CONFIRM) {
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

            Registry<std::string, IControlProvider *> &controlProviders =
                static_cast<ControlProviderManager &>(Game::INSTANCE->getControlProviderManager()).getControlProviders();

            for (int i = 0; i < 4; i += 1)
                settings.players[i].mappings = controlProviders.get(settings.players[i].controlProvider)->
                    getPlayerMappings(i);

            SceneManager::Scene &playScene = scene.manager->getScene(SceneManager::PLAY_ID);
            playScene.scene->clear();
            World *world = manager.create(settings);
            PlayScene::initialize(playScene);
            scene.manager->setActiveScene(SceneManager::PLAY_ID);
            world->focusECS(playScene);

            return true;
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_COMBO_BOX_CHANGED
        && event.GUIEvent.Caller->getID() >= 42) {
            irr::gui::IGUIComboBox *comboBox = static_cast<irr::gui::IGUIComboBox *>(
                event.GUIEvent.Caller);

            if (comboBox->getSelected() != -1) {
                int playerIdx = (comboBox->getID() - 42) / 10;
                std::wstring tmp = std::wstring(comboBox->getItem(comboBox->getSelected()));

                areas[playerIdx][settings.players[playerIdx].controlProvider]->setVisible(false);
                settings.players[playerIdx].controlProvider = std::string(tmp.begin(), tmp.end());
                areas[playerIdx][settings.players[playerIdx].controlProvider]->setVisible(true);
            }

            return true;
        }

        Registry<std::string, IControlProvider *> &controlProviders =
            static_cast<ControlProviderManager &>(Game::INSTANCE->getControlProviderManager()).getControlProviders();

        for (int i = 0; i < 4; i += 1) {
            if (controlProviders.get(settings.players[i].controlProvider)->
                onConfigurationEvent(event,
                areas[i][settings.players[i].controlProvider], i)) {
                    return true;
            }
        }

        return false;
    }

}