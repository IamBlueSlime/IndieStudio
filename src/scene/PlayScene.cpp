/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene PlayScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/PlayScene.hpp"

namespace IndieStudio {

    const int PlayScene::SQUARED_SIZE = 20;
    const float PlayScene::FLOOR_Y = 50.0f;

    void PlayScene::initialize(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        scene.scene->clear();

        WorldSettings &settings = static_cast<WorldManager &>(
            Game::INSTANCE->getWorldManager()).getLoadedWorld()->getSettings();
        irr::scene::ICameraSceneNode *camera = scene.scene->addCameraSceneNode(0);
        camera->setPosition(irr::core::vector3df(
            SQUARED_SIZE * (settings.width / 2), 50 + SQUARED_SIZE * settings.width, SQUARED_SIZE * 3));
        camera->setTarget(irr::core::vector3df(
            SQUARED_SIZE * (settings.width / 2), 50, SQUARED_SIZE * (settings.height / 2)));
		camera->setFarValue(10000);

        setupWaterBackground(scene);
        setupLight(scene);
        setupTravelling(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };
    }

    void PlayScene::setupWaterBackground(SceneManager::Scene &scene)
    {
        irr::scene::IAnimatedMesh* mesh = scene.scene->addHillPlaneMesh("myHill",
            irr::core::dimension2df(20, 20), irr::core::dimension2du(40, 40), 0, 0,
            irr::core::dimension2df(0, 0), irr::core::dimension2df(10, 10));

        irr::scene::ISceneNode* water = scene.scene->addWaterSurfaceSceneNode(
            mesh->getMesh(0), 1.0f, 500.0f, 10.0f);
        water->setPosition(irr::core::vector3df(150, 7, 100));
        water->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/water_stones.jpg").content);
        water->setMaterialTexture(1, scene.manager->textureManager.getTexture("assets/textures/water.jpg").content);
        water->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);
        scene.scene->addSkyDomeSceneNode(scene.manager->textureManager.getTexture("assets/textures/skydome.jpg").content);
        //node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
        scene.scene->setShadowColor(irr::video::SColor(150, 0, 0, 0));
    }

    void PlayScene::setupLight(SceneManager::Scene &scene)
    {
        irr::scene::ISceneNode* light = scene.scene->addLightSceneNode(
            0, irr::core::vector3df(355, 250, 50),
		    irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
        light = scene.scene->addBillboardSceneNode(light, irr::core::dimension2d<irr::f32>(50, 50));
        light->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        light->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        light->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/particlewhite.bmp").content);
        scene.scene->addLightSceneNode(0, irr::core::vector3df(0, -50, 0),
            irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
    }

    void PlayScene::setupTravelling(SceneManager::Scene &scene)
    {
        irr::scene::ICameraSceneNode *cam = scene.scene->getActiveCamera();
        irr::core::vector3df pos(cam->getAbsolutePosition());
        irr::core::array<irr::core::vector3df> points;

        for (int y = 250; y >= 0; y -= 15)
            points.push_back(irr::core::vector3df(pos.X, -y, pos.Z -
                std::sqrt(std::abs(std::pow(y, 2) - std::pow(250, 2)))));

        cam->setPosition(irr::core::vector3df(pos.X, -300, pos.Z - 250));
        points.push_back(pos);

        irr::scene::ISceneNodeAnimator* sa = scene.scene->createFollowSplineAnimator(
            Game::getDevice()->getTimer()->getTime(), points, 5, 0.5F, false);
        cam->addAnimator(sa);
        sa->drop();
    }

    bool PlayScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
                return true;
            }
        }

        World *world = static_cast<WorldManager &>(
            Game::INSTANCE->getWorldManager()).getLoadedWorld();

        ECS::Event::EventData data;
        switch (event.EventType) {
            case irr::EEVENT_TYPE::EET_KEY_INPUT_EVENT:
                data.type = ECS::Event::EventType::KEYBOARD_EVENT;
                data.keyInput = event.KeyInput;
                world->forwardEvent(data);
                return true;

            default:
                break;
        }

        return false;
    }

}