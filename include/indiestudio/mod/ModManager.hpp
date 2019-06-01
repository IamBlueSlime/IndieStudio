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
#include "indiestudio/mod/AMod.hpp"
#include "indiestudio/mod/LibraryWrapper.hpp"

namespace IndieStudio {

    class Game;

    class ModManager {
    public:
        ModManager(Game &game);

        using UAMod = std::unique_ptr<AMod>;

        void init();
        void flush();

        const std::unordered_map<AMod::Descriptor *, UAMod> &getMods() const
        {
            return this->mods;
        }

    protected:
    private:
        Game &game;
        Logger logger;
        std::vector<LibraryWrapper> modsLibraries;
        std::unordered_map<AMod::Descriptor *, UAMod> mods;

        void loadMod(const std::string &path);
        LibraryWrapper openLibrary(const std::string &path);
    };

}
