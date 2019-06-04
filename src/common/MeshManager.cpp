/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common MeshManager.cpp
*/

#include "indiestudio/common/MeshManager.hpp"
#include "indiestudio/Game.hpp"
#include "indiestudio/Singleton.hpp"


namespace IndieStudio {

    MeshManager::MeshManager() : logger("meshmanager")
    {}

    void MeshManager::registerMesh(std::string id)
    {
        Mesh mesh;
        mesh.content = Singleton::getDevice()->getSceneManager()->getMesh(id.c_str());

        if (!mesh.content)
            throw std::runtime_error("Failed to load the mesh " + id);

        this->registerMesh(id, mesh);
    }

    void MeshManager::registerMesh(std::string id, Mesh &mesh,
        bool overrideExisting)
    {
        this->registry.add(id, std::move(mesh), overrideExisting);
        this->logger.debug("Registered mesh '" + id + "'.");
    }

    MeshManager::Mesh &MeshManager::getMesh(const std::string &id)
    {
        if (!this->registry.has(id))
            throw std::runtime_error("Failed to find mesh with id " + id);

        return this->registry.get(id);
    }

}