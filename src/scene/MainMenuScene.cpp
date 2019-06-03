/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene MainMenuScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/MainMenuScene.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/scene/PlayScene.hpp"

namespace IndieStudio {

    sf::Sound MainMenuScene::BACKGROUND_MUSIC = sf::Sound();

    void MainMenuScene::initialize(SceneManager::Scene &scene)
    {
        SoundManager &soundManager = static_cast<SoundManager &>(
            Game::INSTANCE->getSoundManager());
        BACKGROUND_MUSIC.setBuffer(soundManager.getSound("assets/sounds/shooting_stars.ogg").buffer);
        BACKGROUND_MUSIC.play();
        BACKGROUND_MUSIC.setPlayingOffset(sf::seconds(21));

        scene.scene->addSkyDomeSceneNode(scene.manager->textureManager.getTexture("assets/textures/skydome.jpg").content);

        scene.scene->addCameraSceneNode(0, irr::core::vector3df(50, 0, 0), irr::core::vector3df(0, 0, 0));

        setupMenu(scene);
        setupCharacterExplosion(scene);
        setupTravelling(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };
    }

    void MainMenuScene::setupMenu(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        guiEnv->addImage(
            scene.manager->textureManager.getTexture("assets/textures/title.png").content, irr::core::position2di(350, 75), true, guiRoot);

        irr::core::vector2di pos(515, 450);
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_PLAY_NEW, L"New Game");
        pos.Y += 60;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_PLAY_LOAD, L"Load Game");
        pos.Y += 60;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_SETTING, L"Settings");
        pos.Y += 60;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_QUIT, L"Quit");
    }

    void MainMenuScene::setupCharacterExplosion(SceneManager::Scene &scene)
    {

        irr::scene::IAnimatedMeshSceneNode* anms = scene.scene->addAnimatedMeshSceneNode(
            scene.scene->getMesh("assets/models/player.md3"), 0, 42);

        if (!anms)
            throw std::runtime_error("Failed to load player.md3");

        anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        anms->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_black.png").content);
        anms->setFrameLoop(0, 27);
        anms->setAnimationSpeed(30);
        anms->setScale(irr::core::vector3df(8.f,8.f,8.f));
        anms->setAnimationSpeed(18);
        anms->setPosition(irr::core::vector3df(0, -5, 0));

        for (int i = 0; i < 150; i++) {
            int randX = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 100 * 15;
            int randY = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 100 * 15;
            int randZ = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 100 * 15;

            irr::scene::ISceneNodeAnimator *anim = scene.scene->createFlyStraightAnimator(
                anms->getAbsolutePosition(), irr::core::vector3df(randX, randY, randZ),
                15000, true, true
            );

            irr::scene::IAnimatedMeshSceneNode *clone = (irr::scene::IAnimatedMeshSceneNode *)anms->clone();
            clone->addAnimator(anim);

            anim = scene.scene->createRotationAnimator(
                irr::core::vector3df(rand() % 3, rand() % 3, rand() % 2 + 1));
            clone->addAnimator(anim);

            int randTexture = rand() % 4;

            if (randTexture == 0)
                clone->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_black.png").content);
            else if (randTexture == 1)
                clone->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_pink.png").content);
            else if (randTexture == 2)
                clone->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_red.png").content);
            else
                clone->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_white.png").content);
        }
    }

    void MainMenuScene::setupTravelling(SceneManager::Scene &scene)
    {
        irr::core::array<irr::core::vector3df> points;
        irr::scene::ICameraSceneNode *cam = scene.scene->getActiveCamera();
        irr::core::vector3df pos(cam->getAbsolutePosition());
        int step = 2;

        while (pos.X >= 0) {
            points.push_back(pos);
            pos.X -= step;
            pos.Z += step;
        }
        while (pos.X >= -50) {
            points.push_back(pos);
            pos.X -= step;
            pos.Z -= step;
        }
        while (pos.X <= 0) {
            points.push_back(pos);
            pos.X += step;
            pos.Z -= step;
        }
        while (pos.X <= 50) {
            points.push_back(pos);
            pos.X += step;
            pos.Z += step;
        }

        auto device = Singleton::getDevice();
        irr::scene::ISceneNodeAnimator* sceneAnimator =
            scene.scene->createFollowSplineAnimator(
                device->getTimer()->getTime(), points, 2);

        cam->addAnimator(sceneAnimator);
        sceneAnimator->drop();
    }

    bool MainMenuScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        (void) scene;
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                Singleton::getDevice()->closeDevice();
                return true;
            }
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller->getID() == BUTTON_ID_PLAY_NEW) {
                scene.manager->setActiveScene(SceneManager::NEW_GAME_ID);
                return true;
            } else if (event.GUIEvent.Caller->getID() == BUTTON_ID_QUIT) {
                Singleton::getDevice()->closeDevice();
                return true;
            }
        }

        return false;
    }

}