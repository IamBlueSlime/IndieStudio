/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod ModManager.hpp
*/

#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/mod/IMod.hpp"
#include "indiestudio/mod/LibraryWrapper.hpp"

namespace IndieStudio {

    class ModManager {
    public:
        ModManager();

        using UIMod = std::unique_ptr<IMod>;

        void reload(const std::string &path = "mods");
        const std::unordered_map<IMod::Descriptor *, UIMod> &getMods() const
        {
            return this->mods;
        }

    protected:
    private:
        Logger logger;
        std::vector<LibraryWrapper> modsLibraries;
        std::unordered_map<IMod::Descriptor *, UIMod> mods;

        void loadMod(const std::string &path);
        LibraryWrapper openLibrary(const std::string &path);
    };

}
