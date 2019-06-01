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

    PlayECS PlayScene::ECS_INSTANCE = PlayECS();

    void PlayScene::initialize(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        scene.scene->clear();
        irr::scene::ICameraSceneNode *camera = scene.scene->addCameraSceneNode(0,
		    irr::core::vector3df(20 * 17 / 2 - 10, 250, 0), irr::core::vector3df(20 * 17 / 2, 0, 0), 1);
		camera->setFarValue(10000);

        setupWaterBackground(scene);
        setupLight(scene);
        setupTravelling(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };

        // TODO: Remove me
        MapPattern pattern(17, 11);
        BasicWorldGenerator gen;
        gen.generate(pattern);
        WorldSettings settings;
        createWorldFromSettings(scene, settings, pattern);
    }

    void PlayScene::createWorldFromSettings(SceneManager::Scene &scene,
        const WorldSettings &settings, const MapPattern &pattern)
    {
        ECS_INSTANCE.flushEntities();

        int w = settings.width;
        int h = settings.height;

        irr::scene::ICameraSceneNode *camera =
            static_cast<irr::scene::ICameraSceneNode *>(scene.scene->getSceneNodeFromId(1));
        camera->setPosition(irr::core::vector3df(SQUARED_SIZE * (w / 2), 50 + SQUARED_SIZE * w, SQUARED_SIZE * 3));
        camera->setTarget(irr::core::vector3df(SQUARED_SIZE * (w / 2), 50, SQUARED_SIZE * (h / 2)));

        pattern.forEach([&](int x, int y, int z, char c) {
            auto &entity = ECS_INSTANCE.addEntity();
            // Add components to the entity
        });

        irr::core::vector3df playerPositions[4] = {
            irr::core::vector3df(SQUARED_SIZE, FLOOR_Y * 1.2, SQUARED_SIZE),
            irr::core::vector3df(SQUARED_SIZE * (w - 2), FLOOR_Y * 1.2, SQUARED_SIZE),
            irr::core::vector3df(SQUARED_SIZE, FLOOR_Y * 1.2, SQUARED_SIZE * (h - 2)),
            irr::core::vector3df(SQUARED_SIZE * (w - 2), FLOOR_Y * 1.2, SQUARED_SIZE * (h - 2))
        };

        for (int i = 0; i < settings.players; i += 1)
            createPlayer(scene, playerPositions[i]);
    }

    void PlayScene::createWorldFromDump(SceneManager::Scene &scene, World *world)
    {
        ECS_INSTANCE.flushEntities();

        int w = world->getSettings().width;
        int h = world->getSettings().height;

        irr::scene::ICameraSceneNode *camera =
            static_cast<irr::scene::ICameraSceneNode *>(scene.scene->getSceneNodeFromId(1));
        camera->setPosition(irr::core::vector3df(SQUARED_SIZE * (w / 2), 50 + SQUARED_SIZE * w, SQUARED_SIZE * 3));
        camera->setTarget(irr::core::vector3df(SQUARED_SIZE * (w / 2), 50, SQUARED_SIZE * (h / 2)));

        // Load entities from the dump
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

    void PlayScene::createPlayer(SceneManager::Scene &scene, irr::core::vector3df position)
    {
       	irr::scene::IAnimatedMeshSceneNode* player = scene.scene->addAnimatedMeshSceneNode(
            scene.scene->getMesh("assets/models/player.md3"), 0);

        player->setMaterialTexture(0, scene.manager->textureManager.getTexture("assets/textures/player_black.png").content);
		player->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		player->setScale(irr::core::vector3df(SQUARED_SIZE, SQUARED_SIZE, SQUARED_SIZE));
		player->setAnimationSpeed(30);
		player->setFrameLoop(27, 76);
		player->setPosition(position);

        auto triangleSelector = scene.scene->createTriangleSelector(player);
		player->setTriangleSelector(triangleSelector);
        triangleSelector->drop();
    }

    bool PlayScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
                return true;
            }
        }
        return false;
    }

}