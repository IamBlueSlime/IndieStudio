/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene PlayScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/PlayScene.hpp"
#include "indiestudio/Singleton.hpp"

namespace IndieStudio {

    const int PlayScene::SQUARED_SIZE = 20;
    const float PlayScene::FLOOR_Y = 50.0f;

    void PlayScene::initialize(SceneManager::Scene &scene)
    {
        // auto guiEnv = scene.scene->getGUIEnvironment();
        // auto guiRoot = scene.gui;

        WorldSettings &settings = static_cast<WorldManager &>(
            Game::INSTANCE->getWorldManager()).getLoadedWorld()->getSettings();
        irr::scene::ICameraSceneNode *camera = scene.scene->addCameraSceneNode(0,
            irr::core::vector3df(SQUARED_SIZE * (settings.width / 2), 50 + SQUARED_SIZE * settings.width - 125, SQUARED_SIZE * 3),
            irr::core::vector3df(SQUARED_SIZE * (settings.width / 2), 50, SQUARED_SIZE * (settings.height / 2)));
		camera->setFarValue(10000);

        setupWaterBackground(scene);
        setupLight(scene);
        setupTravelling(scene);
        setupOverlay(scene);

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
        scene.scene->setShadowColor(irr::video::SColor(100, 0, 0, 0));
        water->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    }

    void PlayScene::setupLight(SceneManager::Scene &scene)
    {
        irr::scene::ISceneNode* lightnode = scene.scene->addLightSceneNode(
            0, irr::core::vector3df(160, 150, 100),
		    irr::video::SColorf(1.0f, 1.0f, 1.0f, 0.5f), 3500.0f);
        irr::scene::ISceneNode* light = scene.scene->addBillboardSceneNode(lightnode, irr::core::dimension2d<irr::f32>(50, 50));
        light->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        light->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        light->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/particlewhite.bmp").content);
        lightnode->addAnimator(scene.scene->createFlyCircleAnimator(lightnode->getAbsolutePosition(), 120, 0.0001));

        lightnode = scene.scene->addLightSceneNode(
            0, irr::core::vector3df(160, 150, 100),
		    irr::video::SColorf(1.0f, 1.0f, 1.0f, 0.5f), 3500.0f);
        light = scene.scene->addBillboardSceneNode(lightnode, irr::core::dimension2d<irr::f32>(50, 50));
        light->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        light->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        light->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/particlewhite.bmp").content);
        lightnode->addAnimator(scene.scene->createFlyCircleAnimator(lightnode->getAbsolutePosition(), 120, 0.0001, {0.0, 1.0, 0.0}, 0.5));

        // scene.scene->addLightSceneNode(0, irr::core::vector3df(0, -50, 0),
            // irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
    }

    void PlayScene::setupTravelling(SceneManager::Scene &scene)
    {
        irr::scene::ICameraSceneNode *cam = scene.scene->getActiveCamera();
        irr::core::vector3df pos(cam->getAbsolutePosition());
        irr::core::array<irr::core::vector3df> points;

        int y = 0;
        y = 450;
        points.push_back(irr::core::vector3df(pos.X + y, pos.Y + y, pos.Z + y));
        points.push_back(irr::core::vector3df(pos.X, pos.Y, pos.Z));

        irr::scene::ISceneNodeAnimator* sa = scene.scene->createFollowSplineAnimator(
            Singleton::getDevice()->getTimer()->getTime(), points, 0.3F, 0.5F, false);
        cam->addAnimator(sa);
        sa->drop();
    }

    void PlayScene::setupOverlay(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        int w = 1280;
        int h = 720;

        irr::gui::IGUIImage *timer = guiEnv->addImage(irr::core::recti(
            {1280 / 2 - (300 / 2), 20}, {(1280 / 2 + (300 / 2)), 20 + 80 }
        ), guiRoot);
        timer->setImage(scene.manager->textureManager.getTexture("assets/textures/timer.png").content);
        timer->setScaleImage(true);

        irr::video::ITexture *iconTextures[4] = {
            scene.manager->textureManager.getTexture("assets/textures/player_black_icon.png").content,
            scene.manager->textureManager.getTexture("assets/textures/player_red_icon.png").content,
            scene.manager->textureManager.getTexture("assets/textures/player_pink_icon.png").content,
            scene.manager->textureManager.getTexture("assets/textures/player_white_icon.png").content
        };

        int wi = 114 / 1.5;
        int hi = 126 / 1.5;
        irr::core::vector2di iconPositions[4] {
            {20, 250}, {w - wi - 20, 250}, {20, 450}, {w - wi - 20, 450}
        };

        for (int i = 0; i < 4; i += 1) {
            irr::gui::IGUIImage *icon = guiEnv->addImage(irr::core::recti(
                iconPositions[i], {iconPositions[i].X + wi, iconPositions[i].Y + hi}
            ), guiRoot);
            icon->setImage(iconTextures[i]);
            icon->setScaleImage(true);

            irr::core::recti bannerRect = irr::core::recti(
                { iconPositions[i].X, iconPositions[i].Y + hi + 10 },
                { iconPositions[i].X + 200, iconPositions[i].Y + hi + 10 + 40 }
            );

            if (i % 2 != 0) {
                bannerRect.UpperLeftCorner.X -= 200 - wi;
                bannerRect.LowerRightCorner.X -= 200 - wi;
            }

            irr::gui::IGUIImage *banner = guiEnv->addImage(bannerRect, guiRoot);

            irr::video::ITexture *bannerTexture = (i % 2 == 0
                ? scene.manager->textureManager.getTexture("assets/textures/player_banner.png").content
                : scene.manager->textureManager.getTexture("assets/textures/player_banner_inverse.png").content
            );

            banner->setImage(bannerTexture);
            banner->setScaleImage(true);
        }
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
                data.type = ECS::Event::EventType::INDIE_KEYBOARD_EVENT;
                data.keyInput = event.KeyInput;
                world->forwardEvent(data);
                return true;

            case irr::EEVENT_TYPE::EET_JOYSTICK_INPUT_EVENT:
                data.type = ECS::Event::EventType::INDIE_JOYSTICK_EVENT;
                data.joystickEvent = event.JoystickEvent;
                world->forwardEvent(data);
                return true;

            case irr::EEVENT_TYPE::EET_MOUSE_INPUT_EVENT:
                data.type = ECS::Event::EventType::INDIE_MOUSE_EVENT;
                data.mouseInput = event.MouseInput;
                world->forwardEvent(data);
                return true;

            default:
                break;
        }

        return false;
    }

}