/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** SceneManager
*/

#include "indiestudio/scene/SceneManager.hpp"

namespace IndieStudio {

    SceneManager::SceneManager()
    {
        irr::IrrlichtDevice *device = IndieStudio::Game::getDevice();

        this->sceneRoot = device->getSceneManager();
        this->guiRoot = device->getGUIEnvironment();
    }

    SceneManager::Scene::Scene(irr::scene::ISceneManager *scene, irr::gui::IGUIEnvironment *gui)
    {
        this->scene = scene;
        this->gui = gui;
    }

    SceneManager::Scene::Scene(const SceneManager::Scene &other)
    {
        this->scene = other.scene;
        this->gui = other.gui;
    }

    SceneManager::Scene &SceneManager::Scene::operator=(const SceneManager::Scene &other)
    {
        this->gui = other.gui;
        this->scene = other.scene;
        return *this;
    }


    SceneManager::Scene &SceneManager::createScene(const std::string &key)
    {
        this->container[key] = Scene(this->sceneRoot->createNewSceneManager(), 0);
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
        Scene scene;

        if (this->active == "none")
            return false;
        if (this->container.find(this->active) == this->container.end())
            return false;
        scene = this->container[this->active];
        if (!scene.gui && !scene.scene)
            return false;
        if (scene.scene)
            scene.scene->drawAll();
        if (scene.gui)
            scene.gui->drawAll();
        return true;
    }

}