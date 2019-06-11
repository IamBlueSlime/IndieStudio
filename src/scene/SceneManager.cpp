/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** SceneManager
*/

#include <iostream>
#include "indiestudio/Game.hpp"
#include "indiestudio/common/Scheduler.hpp"
#include "indiestudio/scene/MainMenuScene.hpp"
#include "indiestudio/scene/NewGameScene.hpp"
#include "indiestudio/scene/PlayScene.hpp"
#include "indiestudio/scene/SceneManager.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/common/Error.hpp"

namespace IndieStudio {

    const std::string SceneManager::MAIN_MENU_ID = "main_menu";
    const std::string SceneManager::NEW_GAME_ID = "new_game";
    const std::string SceneManager::LOAD_GAME_ID = "load_game";
    const std::string SceneManager::SETTINGS_ID = "settings";
    const std::string SceneManager::PLAY_ID = "play";

    SceneManager::SceneManager(ITextureManager *textureManager)
        : eventReceiver(std::make_unique<EventReceiver>(*this))
    {
        this->textureManager = textureManager;
        irr::IrrlichtDevice *device = IndieStudio::Singleton::getDevice();
        device->setEventReceiver(this->eventReceiver.get());

        this->sceneRoot = device->getSceneManager();
        this->guiRoot = device->getGUIEnvironment();
    }

    SceneManager::Scene::Scene(SceneManager *manager, irr::scene::ISceneManager *scene,
        irr::gui::IGUITab *gui)
    {
        this->manager = manager;
        this->scene = scene;
        this->gui = gui;
    }

    SceneManager::Scene::Scene(const SceneManager::Scene &other)
    {
        this->manager = other.manager;
        this->scene = other.scene;
        this->gui = other.gui;
    }

    SceneManager::Scene &SceneManager::Scene::operator=(const SceneManager::Scene &other)
    {
        this->manager = other.manager;
        this->gui = other.gui;
        this->scene = other.scene;
        return *this;
    }

    void SceneManager::init()
    {
        meshManager.registerMesh("assets/models/cube.obj");
        meshManager.registerMesh("assets/models/player.md3");
        meshManager.registerMesh("assets/models/jesus.md3");

        irr::gui::IGUIFont *font = static_cast<FontManager &>(
            Game::INSTANCE->getFontManager()).getFont("assets/fonts/roboto_regular.png").content;
        this->guiRoot->getSkin()->setFont(font);

        MainMenuScene::initialize(this->createScene(MAIN_MENU_ID));
        NewGameScene::initialize(this->createScene(NEW_GAME_ID));
        this->createScene(PLAY_ID);
        this->setActiveScene(MAIN_MENU_ID);
    }

    SceneManager::Scene &SceneManager::createScene(const std::string &key)
    {
        irr::core::dimension2du screenSize = IndieStudio::Singleton::getDevice()->getVideoDriver()->getScreenSize();
        this->container[key] = Scene(this, this->sceneRoot->createNewSceneManager(),
            this->guiRoot->addTab(irr::core::recti(0, 0, screenSize.Width, screenSize.Height),
            0, this->container.size()));
        if (this->container.find(key) == this->container.end())
            throw IndieError("Failed to create scene");
        this->setActiveScene(key);
        return this->container[key];
    }

    void SceneManager::subscribeEvent(std::function<bool(const irr::SEvent &)> onEvent)
    {
        if (this->container.find(this->active) == this->container.end())
            throw IndieError("subscribeEvent: Failed to get actve scene");
        this->container[this->active].onEvent = onEvent;
    }

    void SceneManager::setActiveScene(const std::string &key)
    {
        if (this->container.find(key) == this->container.end())
            throw IndieError("setActiveScene: Failed to create scene");
        for (auto it = this->container.begin(); it != this->container.end(); ++it)
            it->second.gui->setVisible(false);
        this->active = key;
        this->container[key].gui->setVisible(true);
    }

    SceneManager::Scene &SceneManager::getScene(const std::string &key)
    {
        if (this->container.find(key) == this->container.end())
            throw IndieError("Cannot find get");
        return this->container[key];
    }

    bool SceneManager::draw()
    {
        if (this->active == "none")
            return false;
        if (this->container.find(this->active) == this->container.end())
            return false;
        Scene &scene = this->container[this->active];
        if (!scene.gui && !scene.scene)
            return false;
        if (scene.scene)
            scene.scene->drawAll();
        if (scene.gui)
            scene.gui->draw();
        Scheduler::tick();
        return true;
    }

    SceneManager::EventReceiver::EventReceiver(SceneManager &manager)
        : manager(manager)
    {}

    bool SceneManager::EventReceiver::OnEvent(const irr::SEvent &event)
    {
        if (this->manager.getActive() == "none")
            return false;
        return this->manager.getContainer().find(this->manager.getActive())->second.onEvent(event);
    }

}
