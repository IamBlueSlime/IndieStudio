/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** SceneManager
*/

#include <iostream>
#include "indiestudio/Game.hpp"
#include "indiestudio/scene/MainMenuScene.hpp"
#include "indiestudio/scene/SceneManager.hpp"

namespace IndieStudio {

    const std::string SceneManager::MAIN_MENU_ID = "main_menu";
    const std::string SceneManager::PLAY_ID = "play";

    SceneManager::SceneManager()
        : eventReceiver(std::make_unique<EventReceiver>(*this))
    {
        irr::IrrlichtDevice *device = IndieStudio::Game::getDevice();
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
        MainMenuScene::initialize(this->createScene(MAIN_MENU_ID));
        this->setActiveScene(MAIN_MENU_ID);
    }

    SceneManager::Scene &SceneManager::createScene(const std::string &key)
    {
        irr::core::dimension2du screenSize = IndieStudio::Game::getDevice()->getVideoDriver()->getScreenSize();
        this->container[key] = Scene(this, this->sceneRoot->createNewSceneManager(), this->guiRoot->addTab(irr::core::recti(0, 0, screenSize.Width, screenSize.Height)));
        if (this->container.find(key) == this->container.end())
            throw std::runtime_error("Failed to create scene");
        this->active = key;
        return this->container[key];
    }

    void SceneManager::setActiveScene(const std::string &key)
    {
        this->active = key;
    }

    SceneManager::Scene &SceneManager::getScene(const std::string &key)
    {
        if (this->container.find(key) == this->container.end())
            throw std::runtime_error("Cannot find get");
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