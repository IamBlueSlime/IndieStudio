/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene PlayScene.cpp
*/

#include <iomanip>
#include <sstream>
#include "indiestudio/Game.hpp"
#include "indiestudio/common/Scheduler.hpp"
#include "indiestudio/scene/PlayScene.hpp"
#include "indiestudio/Singleton.hpp"

namespace IndieStudio {

    const float PlayScene::FLOOR_Y = 50.0f;

    sf::Sound PlayScene::COUNTDOWN_SOUND = sf::Sound();
    sf::Sound PlayScene::BACKGROUND_MUSIC = sf::Sound();

    std::size_t PlayScene::TIMER_TASK = 0;

    void PlayScene::initialize(SceneManager::Scene &scene)
    {
        irr::core::list<irr::gui::IGUIElement *> children = scene.gui->getChildren();

        for (irr::gui::IGUIElement *element : children)
            scene.gui->removeChild(element);

        WorldSettings &settings = static_cast<WorldManager &>(
            Game::INSTANCE->getWorldManager()).getLoadedWorldImpl()->getSettings();
        irr::scene::ICameraSceneNode *camera = scene.scene->addCameraSceneNode(0,
            irr::core::vector3df(Constants::TILE_SIZE_FACTOR * (settings.width / 2), 50 + Constants::TILE_SIZE_FACTOR * settings.width - 125, Constants::TILE_SIZE_FACTOR * 3),
            irr::core::vector3df(Constants::TILE_SIZE_FACTOR * (settings.width / 2), 50, Constants::TILE_SIZE_FACTOR * (settings.height / 2)));
		camera->setFarValue(10000);

        setupWaterBackground(scene);
        setupLight(scene);
        setupTravelling(scene);
        setupOverlay(scene);
        setupCountdown(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };
    }

    void PlayScene::updateStats(SceneManager::Scene &scene, int playerIdx,
        const ECS::Component::Stat &stats)
    {
        irr::gui::IGUIStaticText *killCount = static_cast<irr::gui::IGUIStaticText *>(
            scene.gui->getElementFromId(424242 + (playerIdx * 10)));
        killCount->setText(std::to_wstring(stats.kill).c_str());

        irr::gui::IGUIStaticText *bombCount = static_cast<irr::gui::IGUIStaticText *>(
            scene.gui->getElementFromId(424242 + (playerIdx * 10) + 1));
        bombCount->setText(std::to_wstring(stats.bomb).c_str());

        irr::gui::IGUIImage *deathCross = static_cast<irr::gui::IGUIImage *>(
            scene.gui->getElementFromId(424242 + (playerIdx * 10) + 2));
        if (!stats.alive) {
            deathCross->setVisible(true);
        }
    }

    void PlayScene::setupWaterBackground(SceneManager::Scene &scene)
    {
        // irr::scene::IAnimatedMesh* mesh = scene.scene->addHillPlaneMesh("myHill",
        //     irr::core::dimension2df(20, 20), irr::core::dimension2du(40, 40), 0, 0,
        //     irr::core::dimension2df(0, 0), irr::core::dimension2df(10, 10));

        // irr::scene::ISceneNode* water = scene.scene->addWaterSurfaceSceneNode(
        //     mesh->getMesh(0), 1.0f, 500.0f, 10.0f);
        // water->setPosition(irr::core::vector3df(150, 7, 100));
        // water->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/water_stones.jpg").content);
        // water->setMaterialTexture(1, scene.manager->textureManager->getTexture("assets/textures/water.jpg").content);
        // water->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);
        scene.scene->addSkyDomeSceneNode(scene.manager->textureManager->getTexture("assets/textures/skydome.jpg").content);
//        //node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
        // scene.scene->setShadowColor(irr::video::SColor(100, 0, 0, 0));
        // water->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    }

    void PlayScene::setupLight(SceneManager::Scene &scene)
    {
        irr::scene::ISceneNode* lightnode = scene.scene->addLightSceneNode(
            0, irr::core::vector3df(160, 150, 100),
		    irr::video::SColorf(1.0f, 1.0f, 1.0f, 0.5f), 3500.0f);
        irr::scene::ISceneNode* light = scene.scene->addBillboardSceneNode(lightnode, irr::core::dimension2d<irr::f32>(50, 50));
        light->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        light->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        light->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/particlewhite.bmp").content);
        lightnode->addAnimator(scene.scene->createFlyCircleAnimator(lightnode->getAbsolutePosition(), 120, 0.0001));

        lightnode = scene.scene->addLightSceneNode(
            0, irr::core::vector3df(160, 150, 100),
		    irr::video::SColorf(1.0f, 1.0f, 1.0f, 0.5f), 3500.0f);
        light = scene.scene->addBillboardSceneNode(lightnode, irr::core::dimension2d<irr::f32>(50, 50));
        light->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        light->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        light->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/particlewhite.bmp").content);
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

        irr::gui::IGUIImage *timer = guiEnv->addImage(irr::core::recti(
            {1280 / 2 - (300 / 2), 20}, {(1280 / 2 + (300 / 2)), 20 + 80 }
        ), guiRoot);
        timer->setImage(scene.manager->textureManager->getTexture("assets/textures/timer.png").content);
        timer->setScaleImage(true);

        irr::gui::IGUIStaticText *timerText = guiEnv->addStaticText(L"00:00",
            timer->getAbsolutePosition(), false, true, guiRoot, 4243);
        timerText->setOverrideFont(Game::INSTANCE->getFontManager().getFont("assets/fonts/roboto_regular_72.png").content);
        timerText->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
        updateTimer(scene);

        int wi = 114 / 1.5;
        int hi = 126 / 1.5;
        irr::core::vector2di iconPositions[4] {
            {20, 250}, {20, 450}, {w - wi - 20, 250}, {w - wi - 20, 450}
        };

        for (int i = 0; i < 4; i += 1) {
            irr::gui::IGUIImage *icon = guiEnv->addImage(irr::core::recti(
                iconPositions[i], {iconPositions[i].X + wi, iconPositions[i].Y + hi}
            ), guiRoot);
            icon->setImage(scene.manager->textureManager->getTexture(
                "assets/textures/player_" + Constants::PLAYER_COLORS[i] + "_icon.png").content);
            icon->setScaleImage(true);

            irr::core::recti bannerRect = irr::core::recti(
                { iconPositions[i].X, iconPositions[i].Y + hi + 10 },
                { iconPositions[i].X + 200, iconPositions[i].Y + hi + 10 + 40 }
            );

            if (i >= 2) {
                bannerRect.UpperLeftCorner.X -= 200 - wi;
                bannerRect.LowerRightCorner.X -= 200 - wi;
            }

            irr::gui::IGUIImage *banner = guiEnv->addImage(bannerRect, guiRoot);

            irr::video::ITexture *bannerTexture = (i >= 2
                ? scene.manager->textureManager->getTexture("assets/textures/player_banner_inverse.png").content
                : scene.manager->textureManager->getTexture("assets/textures/player_banner.png").content
            );

            banner->setImage(bannerTexture);
            banner->setScaleImage(true);

            irr::core::recti controlProviderIconRect = irr::core::recti(
                {iconPositions[i].X + wi + 10, iconPositions[i].Y + hi - 24},
                {iconPositions[i].X + wi + 10 + 24, iconPositions[i].Y + hi}
            );

            if (i >= 2) {
                controlProviderIconRect.UpperLeftCorner.X -= wi + 10 + 24 + 10;
                controlProviderIconRect.LowerRightCorner.X -= wi + 10 + 24 + 10;
            }

            irr::gui::IGUIImage *controlProviderIcon = guiEnv->addImage(controlProviderIconRect, guiRoot);
            controlProviderIcon->setImage(scene.manager->textureManager->getTexture(
                static_cast<WorldManager &>(Game::INSTANCE->getWorldManager()).getLoadedWorldImpl()->
                getSettings().players[i].controlProviderPtr->getIconPath()).content);
            controlProviderIcon->setScaleImage(true);

            irr::gui::IGUIImage *killIcon = guiEnv->addImage(irr::core::recti(
                {bannerRect.UpperLeftCorner.X + 30, bannerRect.UpperLeftCorner.Y + 8},
                {bannerRect.UpperLeftCorner.X + 30 + 24, bannerRect.UpperLeftCorner.Y + 8 + 24}
            ), guiRoot);
            killIcon->setImage(scene.manager->textureManager->getTexture("assets/textures/icons/bomberman.png").content);
            killIcon->setScaleImage(true);

            irr::gui::IGUIStaticText *killCount = guiEnv->addStaticText(L"0", irr::core::recti(
                {bannerRect.UpperLeftCorner.X + 60, bannerRect.UpperLeftCorner.Y + 12},
                {bannerRect.UpperLeftCorner.X + 60 + 42, bannerRect.UpperLeftCorner.Y + 12 + 24}
            ), false, true, guiRoot, 424242 + (i * 10));
            killCount->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

            irr::gui::IGUIImage *bombIcon = guiEnv->addImage(irr::core::recti(
                {bannerRect.UpperLeftCorner.X + 110, bannerRect.UpperLeftCorner.Y + 8},
                {bannerRect.UpperLeftCorner.X + 110 + 24, bannerRect.UpperLeftCorner.Y + 8 + 24}
            ), guiRoot);
            bombIcon->setImage(scene.manager->textureManager->getTexture("assets/textures/icons/bomb.png").content);
            bombIcon->setScaleImage(true);

            irr::gui::IGUIStaticText *bombCount = guiEnv->addStaticText(L"0", irr::core::recti(
                {bannerRect.UpperLeftCorner.X + 140, bannerRect.UpperLeftCorner.Y + 12},
                {bannerRect.UpperLeftCorner.X + 140 + 42, bannerRect.UpperLeftCorner.Y + 12 + 24}
            ), false, true, guiRoot, 424242 + (i * 10) + 1);
            bombCount->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

            irr::gui::IGUIImage *deathCross = guiEnv->addImage(irr::core::recti(
                iconPositions[i], {iconPositions[i].X + wi, iconPositions[i].Y + hi}
            ), guiRoot, 424242 + (i * 10) + 2);
            deathCross->setImage(scene.manager->textureManager->getTexture(
                "assets/textures/icons/bomb.png").content);
            deathCross->setScaleImage(true);
            deathCross->setVisible(false);
        }
    }

    void PlayScene::setupCountdown(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        int w = 1280;
        int h = 720;

        COUNTDOWN_SOUND.setBuffer(static_cast<SoundManager &>(
            Game::INSTANCE->getSoundManager()).getSound("assets/sounds/countdown.ogg").buffer);
        COUNTDOWN_SOUND.play();

        BACKGROUND_MUSIC.setBuffer(static_cast<SoundManager &>(
            Game::INSTANCE->getSoundManager()).getSound("assets/sounds/bg_music_1.wav").buffer);
        BACKGROUND_MUSIC.setLoop(true);
        BACKGROUND_MUSIC.setVolume(30.0f);

        irr::gui::IGUIStaticText *countdown = guiEnv->addStaticText(L"5", irr::core::recti(
            {w / 2 - 20, h / 2 - 40}, { w / 2 + 30, h / 2 + 40}
        ), false, true, guiRoot, 4242);
        countdown->setOverrideFont(Game::INSTANCE->getFontManager().getFont("assets/fonts/roboto_regular_72.png").content);
        countdown->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

        Scheduler::schedule(1000, [&]() {
            irr::gui::IGUIStaticText *countdownRef = static_cast<irr::gui::IGUIStaticText *>
                (scene.gui->getElementFromId(4242));

            if (countdownRef->getText()[0] == L'0') {
                scene.gui->removeChild(countdownRef);

                BACKGROUND_MUSIC.play();

                TIMER_TASK = Scheduler::schedule(1000, [&]() {
                    World *world = static_cast<WorldManager &>(
                        Game::INSTANCE->getWorldManager()).getLoadedWorldImpl();

                    world->getSettings().elapsedSeconds += 1;
                    updateTimer(scene);
                    return true;
                });

                return false;
            }

            std::wstring newText;
            newText += countdownRef->getText()[0] - 1;
            countdownRef->setText(newText.c_str());

            return true;
        });
    }

    void PlayScene::updateTimer(SceneManager::Scene &scene)
    {
         World *world = static_cast<WorldManager &>(
            Game::INSTANCE->getWorldManager()).getLoadedWorldImpl();

        irr::gui::IGUIStaticText *timerRef = static_cast<irr::gui::IGUIStaticText *>
            (scene.gui->getElementFromId(4243));

        int minutes = std::floor(world->getSettings().elapsedSeconds / 60);
        int seconds = world->getSettings().elapsedSeconds % 60;

        std::stringstream timerText;
        timerText << std::setfill('0') << std::setw(2);
        timerText << minutes << ":";
        timerText << std::setfill('0') << std::setw(2);
        timerText << seconds;
        std::string timerTextDone = timerText.str();

        timerRef->setText(std::wstring(timerTextDone.begin(), timerTextDone.end()).c_str());

        irr::core::recti pos = timerRef->getRelativePosition();
        timerRef->setRelativePosition(irr::core::recti(
            {1280 / 2 - (timerRef->getTextWidth() / 2), pos.UpperLeftCorner.Y},
            {1280 / 2 + (timerRef->getTextWidth() / 2), pos.LowerRightCorner.Y}
        ));
    }

    bool PlayScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        WorldManager &worldManager = static_cast<WorldManager &>(
            Game::INSTANCE->getWorldManager());
        World *world = worldManager.getLoadedWorldImpl();

        if (world->getSettings().elapsedSeconds == 0)
            return false;

        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                COUNTDOWN_SOUND.stop();
                BACKGROUND_MUSIC.stop();
                Scheduler::stopTask(PlayScene::TIMER_TASK);

                worldManager.save("./snapshot.bmm", world);

                for (int i = 0; i < 4; i += 1)
                    world->getSettings().players[i].controlProviderPtr->onWorldQuit(i);

                scene.manager->setActiveScene(SceneManager::MAIN_MENU_ID);
                return true;
            }
        }

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