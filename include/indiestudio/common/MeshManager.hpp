/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common MeshManager.hpp
*/

#pragma once

#include "indiestudio/common/IMeshManager.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/common/Registry.hpp"

namespace IndieStudio {

    class MeshManager : public IMeshManager {
	public:
		MeshManager();

        void registerMesh(std::string id) override;
        void registerMesh(std::string id, Mesh &Mesh,
            bool overrideExisting = false) override;
        Mesh &getMesh(const std::string &id);

	protected:
	private:
        Logger logger;
        Registry<std::string, Mesh> registry;
    };

}