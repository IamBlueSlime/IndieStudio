/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene MainMenuScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/MainMenuScene.hpp"

namespace IndieStudio {

    void MainMenuScene::initialize(SceneManager::Scene &scene)
    {
        auto driver = scene.scene->getVideoDriver();
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;
        
        scene.scene->addSkyDomeSceneNode(
            driver->getTexture("assets/textures/skydome.jpg"));
        
        scene.scene->addCameraSceneNode(0, irr::core::vector3df(50, 0, 0));

        guiEnv->addImage(driver->getTexture("assets/textures/title.png"),
            irr::core::position2di(350, 75), true, guiRoot);

        setupCharacterExplosion(scene);
        setupTravelling(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };
    }

    void MainMenuScene::setupCharacterExplosion(SceneManager::Scene &scene)
    {
        auto driver = scene.scene->getVideoDriver();

        irr::scene::IAnimatedMeshSceneNode* anms = scene.scene->addAnimatedMeshSceneNode(
            scene.scene->getMesh("assets/models/player.md3"), 0, 42);

        if (!anms)
            throw std::runtime_error("Failed to load player.md3");

        anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        anms->setMaterialTexture(0, driver->getTexture("assets/textures/player_black.png"));
        anms->setScale(irr::core::vector3df(8.f,8.f,8.f));
        anms->setAnimationSpeed(18);
        anms->setPosition(irr::core::vector3df(0, 0, 0));

        for (int i = 0; i < 100; i++) {
            int randX = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 985 * 15;
            int randY = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 985 * 15;
            int randZ = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 985 * 15;

            irr::scene::ISceneNodeAnimator *anim = scene.scene->createFlyStraightAnimator(
                anms->getAbsolutePosition(), irr::core::vector3df(randX, randY, randZ),
                1000000
            );

            irr::scene::ISceneNode *clone = anms->clone();
            clone->addAnimator(anim);

            anim = scene.scene->createRotationAnimator(
                irr::core::vector3df(rand() % 3, rand() % 3, 1));
            clone->addAnimator(anim);

            int randTexture = rand() % 4;

            if (randTexture == 0)
                clone->setMaterialTexture(0, driver->getTexture("assets/textures/player_black.png"));
            else if (randTexture == 1)
                clone->setMaterialTexture(0, driver->getTexture("assets/textures/player_pink.png"));
            else if (randTexture == 2)
                clone->setMaterialTexture(0, driver->getTexture("assets/textures/player_red.png"));
            else
                clone->setMaterialTexture(0, driver->getTexture("assets/textures/player_white.png"));
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

        auto device = Game::getDevice();
        irr::scene::ISceneNodeAnimator* sceneAnimator =
            scene.scene->createFollowSplineAnimator(
                device->getTimer()->getTime(), points, 2);

        cam->addAnimator(sceneAnimator);
        sceneAnimator->drop();
    }

    bool MainMenuScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                Game::getDevice()->closeDevice();
                return true;
            }
        }

        return false;
    }

}