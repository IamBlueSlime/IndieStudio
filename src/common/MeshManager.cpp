/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common MeshManager.cpp
*/

#include "indiestudio/common/MeshManager.hpp"
#include "indiestudio/Game.hpp"

namespace IndieStudio {

    MeshManager::MeshManager() : logger("Meshmanager")
    {}

    void MeshManager::registerMesh(std::string id)
    {
        Mesh Mesh;
        Mesh.content = Game::getDevice()->getSceneManager()->getMesh(id.c_str());

        if (!Mesh.content)
            throw std::runtime_error("Failed to load the Mesh " + id);

        this->registerMesh(id, Mesh);
    }

    void MeshManager::registerMesh(std::string id, Mesh &Mesh,
        bool overrideExisting)
    {
        this->registry.add(id, std::move(Mesh), overrideExisting);
        this->logger.debug("Registered Mesh '" + id + "'.");
    }

    MeshManager::Mesh &MeshManager::getMesh(const std::string &id)
    {
        if (!this->registry.has(id))
            throw std::runtime_error("Failed to find Mesh with id " + id);

        return this->registry.get(id);
    }

}